/* 
 * File:   MemSat_Comms.h
 * Author: Ryan Hare
 *
 */

#ifndef MEMSAT_COMMS_H
#define	MEMSAT_COMMS_H

#define SYNC_1              0x48    //Define the sync bits for sending and receiving UART
#define SYNC_2              0x65

#define ACKNOWLEDGE         0x0A    //Define the "Acknowledged" and "not acknowledged" response bits from the comms board
#define NOT_ACKNOWLEDGE     0xFF

#define SEND_COMMAND        0x10    //Define the op-codes for sending commands and receiving responses
#define RECEIVE_RESPONSE    0x20

#define COMMAND_NO_OP       0x01    //Define the op-codes for the various commands
#define COMMAND_RESET       0x02
#define TRANSMIT_DATA       0x03
#define RECEIVE_DATA        0x04

#define GET_TRANSCEIVER     0x05
#define SET_TRANSCEIVER     0x06
#define TELEMETRY           0x07
#define WRITE_FLASH         0x08
#define RF_CONFIGURE        0x09
#define SET_BEACON_DATA     0x10
#define BEACON_CONFIGURE    0x11
#define READ_FIRMWARE_REV   0x12
#define DIO_KEY_WRITE       0x13
#define FIRMWARE_UPDATE     0x14
#define FIRMWARE_PACKET     0x15
#include "system.h"
#include "MemSat_SYSTEM.h"

typedef struct{
    uint8_t interface_baud_rate;    //Radio interface baud rate
    uint8_t tx_power_amp_level;     //Tx Power amp level
    uint8_t rx_rf_baud_rate;        //Radio Rx RF baud rate
    uint8_t tx_rf_baud_rate;        //Radio Tx RF baud rate
    uint8_t rx_modulation;
    uint8_t tx_modulation;
    uint32_t rx_freq;               //Channel Rx frequency
    uint32_t tx_freq;               //Channel Tx frequency
    unsigned char source[6];        //AX25 mode source call sign
    unsigned char destination[6];   //AX25 mode destination call sign
    uint16_t tx_preamble;           //AX25 mode Tx preamble byte length
    uint16_t tx_postamble;          //AX25 mode Tx postamble byte length
    uint16_t function_config;       //Radio configuration discrete behaviors
    uint16_t function_config2;      //Radio configuration discrete behaviors 2
} RadioConfig_t;

typedef struct{
    uint16_t op_counter;
    uint16_t msp430_temp;
    uint8_t time_count[3];
    uint8_t rssi;
    uint32_t bytes_received;
    uint32_t bytes_transmitted;
} TelemetryStructure_t;

typedef struct{
    uint8_t front_end_level;        //0 to 63
    uint8_t tx_power_amp_level;     //0 to 255, non-linear
    uint32_t tx_frequency_offset;   //up to 20kHz
    uint32_t rx_frequency_offset;   //up to 20kHz
} RadioRFConfig_t;

typedef struct{
    uint8_t beacon_interval; //0 is off, 2.5 seconds per LSB
} RadioBeaconConfig_t;

uint8_t COMMS_CHANNEL = 0x03;   //Define the UART channel for the comms board

char headerBuffer[8];           //8 byte char array for header buffer
char *headerPtr = headerBuffer;
char dataBuffer[255];           //255 byte char array for data buffer
char *dataPtr = dataBuffer;
char dataChecksumBuffer[2];  //2 byte char array for data checksum buffer
char *dataChecksumPtr = dataChecksumBuffer;
char bufferPos = 0x00;          //char to keep track of positions within the buffers
char *posPtr = &bufferPos;
char commsState = 0x00;         //state machine characters
char nextCommsState = 0x00;
char tempChar = 0x00;

void Comms_Init();                  //Init comms
void Calculate_Checksum(char *Buffer, char *CHK_A, char *CHK_B, uint8_t size); //Checksum algorithm
void Comms_Send_Command(char command); //Send a basic command
void Comms_No_Op();                 //Send a no-op to the comms board
void Comms_Reset();                 //Send the reset signal to the comms board
void Comms_Send_Data(char *data, char command);   //Send data to the comms board for transmission
void Comms_Transmit(char *data);
void Comms_Get_Transceiver_Config();//Tell the comms board to send its transceiver configuration struct
void Comms_Telemetry();             //Get telemetry data
void Comms_RxHandler(char C);       //Data receiver handler function
void Comms_Read_Firmware_Rev();     //Read the firmware revision
void Comms_Set_Beacon_Data(char *data);//Set the beacon data content
void Comms_Write_Flash(char * data);//Write to flash on comms board. Only takes 2 bytes
void Comms_DIO_Key_Write(char *data);//Write DIO key to comms board
void Comms_Firmware_Update(char *data);//Write a firmware update
void Comms_Firmware_Packet(char *data);//Write a firmware packet
void Comms_Set_Transceiver_Config(RadioConfig_t *radio); //Set the transceiver configuration
void Comms_RF_Configure(RadioRFConfig_t *radio);    //Low level RF configuration
void Comms_Beacon_Configure(RadioBeaconConfig_t *radio);   //Beacon configuration

#endif	/* MEMSAT_COMMS_H */

