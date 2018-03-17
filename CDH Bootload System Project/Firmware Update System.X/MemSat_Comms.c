/* 
 * File:   MemSat_Comms.c
 * Author: Ryan Hare
 *
 * Function: UART functions for communication with the comms board
 */

#include "MemSat_Comms.h"

void Comms_Init()
{
    UART_RegisterReceiver(COMMS_CHANNEL, Comms_RxHandler);
}

void Calculate_Checksum(char *Buffer, char *CHK_A, char *CHK_B, uint8_t size) //8-bit fletcher algorithm to calculate the checksums
{
    uint8_t i = 0, sum_A = 0, sum_B = 0;
    CHK_A = 0;
    CHK_B = 0;
    
    for (i = 0; i < size; i++)
    {
        sum_A = (sum_A + *(Buffer + i)) % 0xFF;
        sum_B = (sum_B + sum_A) % 0xFF;
    }
    *CHK_A = 0xFF - ((sum_A + sum_B) % 0xFF);
    *CHK_B = 0xFF - ((sum_A + *CHK_A) % 0xFF);
}

void Comms_Send_Command(char command)
{
    char buffer[8], temp[4];
    char *tempPtr = temp, *bufferPtr = buffer;
    uint8_t i = 0;
    buffer[0] = SYNC_1;     //Assign the data buffer
    buffer[1] = SYNC_2;
    buffer[2] = SEND_COMMAND;
    buffer[3] = command;
    buffer[4] = 0x00;
    buffer[5] = 0x00;
    for (i = 0; i < sizeof(temp); i++)  //The header checksum excludes the synch bits
        temp[i] = buffer[i + 2];
    Calculate_Checksum(tempPtr, (bufferPtr + 6), (bufferPtr + 7), sizeof(temp));  //Calculate header checksum
    
    UART_Write(COMMS_CHANNEL, buffer, 0x08); //Send the data over serial
}

void Comms_No_Op()
{
    Comms_Send_Command(COMMAND_NO_OP);
}

void Comms_Reset()
{
    Comms_Send_Command(COMMAND_RESET);
}

void Comms_Send_Data(char *data, char command)
{
    unsigned short size = sizeof(data);
    uint8_t i = 0, j = 0;
    
    char buffer[size + 10], temp[4];     //Assign the data buffer.
    char *tempPtr = temp, *bufferPtr = buffer;
    
    buffer[0] = SYNC_1;
    buffer[1] = SYNC_2;
    buffer[2] = SEND_COMMAND;
    buffer[3] = command;
    buffer[4] = size >> 8;
    buffer[5] = size;
    for (i = 0; i < sizeof(temp); i++)  //Exclude synch bits for calculating checksum
        temp[i] = buffer[i + 2];
    Calculate_Checksum(tempPtr, (bufferPtr + 6), (bufferPtr + 7), sizeof(temp));  //Calculate header checksum
    
    for (j = 0; j < size; j++)  //Put the data into the buffer
        buffer[j + 8] = data[j];
    
    Calculate_Checksum(bufferPtr, (bufferPtr + (size + 8)), (bufferPtr + (size + 9)), sizeof(buffer));  //Calculate the checksum for the entire payload
    
    UART_Write(COMMS_CHANNEL, data, size + 10); //Send the data over serial
}

void Comms_Transmit(char *data)
{
    Comms_Send_Data(data, TRANSMIT_DATA);
}

void Comms_Get_Transceiver_Config()
{
    Comms_Send_Command(GET_TRANSCEIVER);
}

void Comms_Telemetry()
{
    Comms_Send_Command(TELEMETRY);
}

void Comms_RxHandler(char C)
{
    switch(commsState)
    {
        case 0x00:                  //Start case
            *posPtr = 1;            //Restart buffer position
            headerBuffer[0] = C;    //Store the first byte in the header buffer
            nextCommsState = 0x01;      //Set next state
            break;
        case 0x01:                  //Storing header case
            *(headerPtr + *posPtr) = C; //Store data byte in header buffer
            *posPtr = *posPtr + 1;              //Increment position
            if (*posPtr < 0x07);    //Until all 8 header bytes have been stored, remain in the same state
            else                    //Once all 8 header bytes have been stored, switch states based on the command being sent
            {
                tempChar = *(headerPtr + 3);    //Store command in tempChar
                switch(tempChar)
                {
                    case 0x04:  //Received data
                        nextCommsState = 0x02;   //Set state 2 as next state
                        break;
                    case 0x05:  //Get transceiver config
                        nextCommsState = 0x02;
                        break;
                    case 0x07:  //Telemetry
                        nextCommsState = 0x02;
                        break;
                    case 0x0C:  //Read firmware rev
                        nextCommsState = 0x02;
                        break;
                    default:    //Any command that doesn't require receiving data (all the acknowledges)
                        nextCommsState = 0x00;
                        break;
                }
                *posPtr = 0;            //Reset position pointer
                tempChar = *(headerPtr + 5);    //Store data length in tempChar
            }
            break;
        case 0x02:  //Receive data case
            *(dataPtr + *posPtr) = C; //Store data byte in data buffer
            *posPtr = *posPtr + 1;              //Increment position
            if (*posPtr < tempChar);//Until an amount of data given by the header has been stored, remain in case 2
            else                    //Once all data has been received, move to case 3
            {
                nextCommsState = 0x03;   //Set next state to 3
                *posPtr = 0;        //Reset position pointer
            }
            break;
        case 0x03:  //Receive data checksum case
            *(dataChecksumPtr + *posPtr) = C;
            *posPtr = *posPtr + 1;
            if (*posPtr < 0x02);    //Do nothing until both checksums have been received
            else
                nextCommsState = 0x00;   //Reset to start state
            break;
            //Could add another state here to make sure the checksums are accurate (using Calculate_Checksum())
        default:
            nextCommsState = 0x00;       //Reset state to zero in default case
            break;
    }
    commsState = nextCommsState;  //Set current state to next state
}

void Comms_Read_Firmware_Rev()
{
    Comms_Send_Command(READ_FIRMWARE_REV);
}

void Comms_Set_Beacon_Data(char *data)
{
    Comms_Send_Data(data, SET_BEACON_DATA);
}

void Comms_Write_Flash(char * data)
{
    Comms_Send_Data(data, WRITE_FLASH);
}

void Comms_DIO_Key_Write(char *data)
{
    Comms_Send_Data(data, DIO_KEY_WRITE);
}

void Comms_Firmware_Update(char *data)
{
    Comms_Send_Data(data, FIRMWARE_UPDATE);
}

void Comms_Firmware_Packet(char *data)
{
    Comms_Send_Data(data, FIRMWARE_PACKET);
}

void Comms_Set_Transceiver_Config(RadioConfig_t *radio)
{
    //These three functions would require casting a structure to a char array and then passing that array into Comms_Send_Data.
    //I could not figure out a way to make a struct into a char array. As such, these final three functions do not work.
    
    char buffer[sizeof(radio)];
    //somehow store radio in buffer here
    Comms_Send_Data(buffer, SET_TRANSCEIVER);
}

void Comms_RF_Configure(RadioRFConfig_t *radio)
{
    char buffer[sizeof(radio)];
    //somehow store radio in buffer here
    Comms_Send_Data(buffer, RF_CONFIGURE);
}

void Comms_Beacon_Configure(RadioBeaconConfig_t * radio)
{
    char buffer[sizeof(radio)];
    //somehow store radio in buffer here
    Comms_Send_Data(buffer, BEACON_CONFIGURE);
}