/* 
 * File:   bootload_system.c
 * Author: Tanner Smith
 *
 * This file is meant to manage incoming and outgoing messages with COMMS for image manipulation, image storage and image processing
 * 
 * Created on March 6, 2018, 9:37 PM
 */

#include <stdint.h>
#include "bootload_system.h"
#include "at25df641a.h"
#include "crc.h"

enum States {IDLE, START_FIRMWARE_UPDATE, START_OF_BLOCK, RECEIVE_DATA, END_OF_BLOCK, END_OF_FIRMWARE, LOAD_NEW_FIRMWARE} current_state;    // Declare states for state machine

static uint8_t transmit_buffer[128];                // Buffer used to store data before transmitting
static uint8_t data_buffer[128];                    // Buffer to store data when reading and writing to flash
static uint8_t subblock_buffer[32];                 // Buffer that keeps track of successful subblock transfers
static uint8_t block_buffer[32];                    // Buffer that keeps track of successful block transfers
static uint16_t crc_subblock_buffer[32];            // Buffer to remember subblock crcs
static uint16_t crc_block_buffer[32];               // Buffer to remember block crcs

uint8_t debug_buffer[256];                   //Debug buffer, get rid of later

static uint8_t currently_processing = 0;            // Flag set when processing message
static uint8_t image_being_stored = 0;              // Current image being stored
static uint8_t number_of_blocks = 0;                // Number of blocks for incoming image
static uint8_t current_block = 0;                   // Curent block being received
static uint8_t ready_to_update = READY_TO_UPDATE;   // Ready to update flag (TODO: Find out when to change this)

static uint8_t i = 0;                               // Loop index pointer 1
static uint8_t j = 0;                               // Loop index pointer 2

/********** Functions **********/

void BOOTLOADSYSTEM_Init(void (*transmitter)(uint8_t));
void BOOTLOADSYSTEM_Receiver(uint8_t * data);
void BOOTLOADSYSTEM_Transmitter(uint8_t * data, uint8_t length);

void (*transmit_single)(uint8_t data);

void PROCESSMSG_FirmwareVersion(uint8_t *data);
uint8_t PROCESSMSG_StartFirmwareUpdate(uint8_t *data);
uint8_t PROCESSMSG_StartBlock(uint8_t *data);
void PROCESSMSG_ReceiveData(uint8_t *data);
uint8_t PROCESSMSG_EndBlock(uint8_t *data);
uint8_t PROCESSMSG_EndFirmware(uint8_t *data);
void PROCESSMSG_LoadFirmware(uint8_t *data);

void SEND_FirmwareVersion(uint8_t image, uint8_t bootloader_version_higher, uint8_t bootloader_version_lower, uint8_t firmware_version_higher, uint8_t firmware_version_lower);
void SEND_StartFirmwareUpdateResponse();
void SEND_StartBlockResponse(uint8_t image_match);
void SEND_EndBlockResponse(uint8_t status, uint8_t* status_buffer, uint8_t status_buffer_size);
void SEND_EndFirmwareResponse(uint8_t status, uint8_t* status_buffer, uint8_t status_buffer_size);
void SEND_LoadNewFirmwareResponse(uint8_t image);
void SEND_Statestatus();


/* Initializes the Bottloader System.
 * Sets intial values and registers receiver and tranceiver with COMMS.
 */
void BOOTLOADSYSTEM_Init(void (*transmitter)(uint8_t)){
    current_state = IDLE;
    transmit_single = transmitter;
}

/* Receiver for Bootload System. Determines command from incoming data 
 * and determine if the command is valid based on current state.
 *
 * @parameter *data         Incoming packet to bootloader system
 */
void BOOTLOADSYSTEM_Receiver(uint8_t * data){
    if(!currently_processing){

        currently_processing = 1;                                                       // Set flag so no more messages are received
        uint8_t command = *data++;                                                      // Store incoming command (always first byte)
        uint8_t transaction;                                                            // Variable to determine what state to move to

        switch(command){
            case GET_FIRMWARE_VERSION_MSG:                                              /********** GET FIRMWARE MESSAGE **********/
                if(current_state == IDLE){                                              // Only works if you are currently in the IDLE state
                    PROCESSMSG_FirmwareVersion(data);
                }
                else{
                    SEND_Statestatus();                                                  // Send status if in wrong state
                }
                break;

            case START_FIRMWARE_UPDATE_MSG:                                             /********** START FIRMWARE UPDATE MESSAGE **********/
                if(current_state == IDLE){                                              // Only valid if you are currently in the IDLE state
                    transaction = PROCESSMSG_StartFirmwareUpdate(data);
                    
                    if(transaction == SUCCESS){                                         // If system is ready to start a firmware update
                        current_state = START_FIRMWARE_UPDATE;                          // Move states to START FIRMWARE UPDATE
                    }
                    else{                                                               // If not ready to start firmware update
                        current_state = IDLE;                                           // Stay in IDLE state
                    }
                }
                else{
                    SEND_Statestatus();                                                  // Send status if in wrong state
                }
                break;

            case START_BLOCK_MSG:                                                       /********** START BLOCK MESSAGE **********/
                if(current_state == START_FIRMWARE_UPDATE || END_OF_BLOCK){             // Can enter start bock from START FIRMWARE STATE (first block received)
                    transaction = PROCESSMSG_StartBlock(data);                          // Or from END OF BLOCK state (not first block received)
                    
                    if(transaction == SUCCESS){                                         // If system is ready to start receiving blocks
                        current_state = START_OF_BLOCK;                                 // Move states to START BLOCK
                    }
                    else{                                                               // If not ready to start receiving blocks
                        current_state = START_FIRMWARE_UPDATE;                          // Stay in START FIRMWARE UPDATE state
                    }
                }
                else{
                    SEND_Statestatus();                                                  // Send status if in wrong state
                }
                break;

            case RECEIVE_DATA_MSG:                                                      /********** RECEIVE DATA MESSAGE **********/
                if(current_state == START_OF_BLOCK || current_state == RECEIVE_DATA){   // Can only receive data if just starting a block (START BLOCK state)
                    PROCESSMSG_ReceiveData(data);                                       // Or if currently receiving subblocks (RECEIVE_DATA state)
                    current_state = RECEIVE_DATA;
                }
                else{
                    SEND_Statestatus();                                                  // Send status if in wrong state
                }
                break;
                
            case END_BLOCK_MSG:                                                         /********** END BLOCK MESSAGE **********/
                if(current_state == RECEIVE_DATA){                                      // Only valid if just finished receiving data (RECEIVE_DATA state)
                    transaction = PROCESSMSG_EndBlock(data);                              

                    if(transaction == SUCCESS){                                         // If all subblocks received correctly
                        current_state = END_OF_BLOCK;                                   // Move to END BLOCK state and wait for new start block or end firmware
                    }
                    else{                                                               // If not all subblocks are received correctly
                        current_state = RECEIVE_DATA;                                   // Stay in RECEIVE DATA state
                    }
                }
                else{
                    SEND_Statestatus();                                                  //  Send status if in wrong state
                }
                break;

            case END_FIRMWARE_MSG:                                                      /********** END FIRMWARE MESSAGE **********/
                if(current_state == END_OF_BLOCK){                                      // Only valid after just ending a block (END BLOCK state)
                    transaction = PROCESSMSG_EndFirmware(data);
                    if(transaction == SUCCESS){                                         // If all blocks were received correctly
                        current_state = IDLE;                                           // Finish firmware update and go to IDLE
                    }
                    else{                                                               // If not all blocks are good
                        current_state = END_OF_BLOCK;                                   // Stay in END BLOCK
                    }
                }
                else{
                    SEND_Statestatus();                                                  //  Send status if in wrong state
                }
                break;

            case LOAD_NEW_FIRMWARE_MSG:                                                 /********** LOAD NEW FIRMWARE MESSAGE **********/
                if(current_state == IDLE){                                              // Only valid from IDLE state
                    PROCESSMSG_LoadFirmware(data);                                      // Set bootload flag and reset
                }
                else{
                    SEND_Statestatus();                                                  //  Send status if in wrong state
                }
                break;
                
            default:
                
                break;
        }
        currently_processing = 0;                                                       // Clear flag beause processing is done and system is ready for new message
    }
}

/* Transmitter for Bootload System
 * 
 * @parameter *data         Outgoing packet to ground system
 * @parameter length        Length of packet
 */
void BOOTLOADSYSTEM_Transmitter(uint8_t * data, uint8_t length){
    for(i = 0; i < length; i++){
        transmit_single(*data++);
    }
}

/*************************** FIRMWARE VERSION INTERACTION ********************************/

/* Processes Get_Firmware_Version command
 * Calculates firmware address location based on image 
 *
 * TODO: FIX ADDRESS CALCULATIONS FOR VERSIONS
 *
 * @parameter *data         incoming packet
 */
void PROCESSMSG_FirmwareVersion(uint8_t *data){
    uint8_t image = *data;                                                          // Grab image of desired firmware version
    uint32_t bootload_version_address = SYSTEM_INFORMATION_BASE_ADDRESS;            // Firmware address will always be first in an image allocated memory space. All images are stored in triplicate
    uint8_t firmware_version_lower = 0;                                             // Firmware version variable
    uint8_t firmware_version_higher = 0; 
    uint8_t bootloader_version_lower = 0;                                           // Bootloader version variable
    uint8_t bootloader_version_higher = 0; 
    at25df641a_read(bootload_version_address, &bootloader_version_lower, 1);                            // Read firmware version
    at25df641a_read(bootload_version_address+1, &bootloader_version_higher, 1);                         // Read firmware version
    at25df641a_read(bootload_version_address+2*image, &firmware_version_lower, 1);                      // Read bootloader version
    at25df641a_read(bootload_version_address+2*image+1, &firmware_version_higher, 1);                   // Read bootloader version
    
    SEND_FirmwareVersion(image, bootloader_version_higher, bootloader_version_lower, firmware_version_higher, firmware_version_lower);              // Send versions to ground station
}

/* Sends Give_Firmware_Version response
 *
 * @parameter image                         specific image
 * @parameter bootloader_version            version of bootloader
 * @parameter firmware_version              version of specific image
 */
void SEND_FirmwareVersion(uint8_t image, uint8_t bootloader_version_higher, uint8_t bootloader_version_lower, uint8_t firmware_version_higher, uint8_t firmware_version_lower){
    transmit_buffer[0] = GIVE_FIRMWARE_VERSION_RESPONSE;
    transmit_buffer[1] = image;
    transmit_buffer[2] = bootloader_version_higher;
    transmit_buffer[3] = bootloader_version_lower;
    transmit_buffer[4] = firmware_version_higher;
    transmit_buffer[5] = firmware_version_lower;
    
    uint8_t length = 6;
    
    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}




/*************************** START FIRMWARE VERSION INTERACTION ********************************/

/* Processes Start_Firmware_Update command
 * Stores system being updated and number of expected blocks
 *
 * @parameter *data         incoming packet
 * @return                  whether or not to continue to next state
 */
uint8_t PROCESSMSG_StartFirmwareUpdate(uint8_t *data){
    image_being_stored = *data++;                       // Grab image number that will be stored
    number_of_blocks = *data++;                          // Grab number of expected blocks 

    for(i = 0; i < 32; i++){                            // Clear all buffers            
        block_buffer[i] = 0;                
        crc_subblock_buffer[i] = 0;            
        crc_block_buffer[i] = 0; 
    }                              
    
    SEND_StartFirmwareUpdateResponse();                 // Send Start Firmware Update Response
    
    if(ready_to_update == READY_TO_UPDATE){
        return SUCCESS;
    }
    else{
        return FAILURE;
    }
}

/* Sends Start_Firmware_Update response
 *
 */
void SEND_StartFirmwareUpdateResponse(){
    transmit_buffer[0] = START_FIRMWARE_UPDATE_RESPONSE;
    transmit_buffer[1] = image_being_stored;
    transmit_buffer[2] = number_of_blocks;
    if(ready_to_update == READY_TO_UPDATE){
        transmit_buffer[3] = READY_TO_UPDATE;
    }
    else{
        transmit_buffer[3] = NOT_READY_TO_UPDATE;
    }
    
    uint8_t length = 4;
    
    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}




/*************************** START OF BLOCK INTERACTION ********************************/

/* Processes Start_Block command
 * Checks if block system and image system matches
 * Store expected number of subblocks
 *
 * @parameter *data         incoming packet
 */
uint8_t PROCESSMSG_StartBlock(uint8_t *data){
    uint8_t image_match = 0;                  // Create pointer to indicate whether image of block corresponds to firmware image
    if(image_being_stored == *data++){
        image_match = 1;                      // If system matches current system to update then systems match
    }
    current_block = *data++;                  // Store the current blocks data
    
    SEND_StartBlockResponse(image_match);     // Send Start Block Response

    if(image_match){
        return SUCCESS;
    }
    else{
        return FAILURE;
    }

}

/* Sends Start_Block response
 *
 * @parameter image_match         1 if block image corresponds to firmware image being received, 0 otherwise
 */
void SEND_StartBlockResponse(uint8_t image_match){
    transmit_buffer[0] = START_BLOCK_RESPONSE;
    transmit_buffer[1] = image_being_stored;
    transmit_buffer[2] = current_block;
    if(image_match){
        transmit_buffer[3] = READY_TO_UPDATE;
    }
    else{
        transmit_buffer[3] = NOT_READY_TO_UPDATE;
    }
    
    uint8_t length = 4;
    
    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}



/*************************** RECEIVE DATA INTERACTION ********************************/


/* Processes Receive_Data command
 * Grabs subblock number and stores address of subblock
 * Stores data in a buffer and check crc
 *
 * @parameter *data         incoming packet
 */
void PROCESSMSG_ReceiveData(uint8_t *data){
    uint8_t subblock_number = *data++;                                              // Stores subblock number
    
    for(i = 0; i < SUBBLOCK_SIZE; i++){
        data_buffer[i] = *data++;                                                      // Store data in buffer
    }
    uint16_t upper_crc = *data++;                                                   // Grab upper CRC value
    uint16_t lower_crc = *data++;                                                   // Grab lower CRC value
    uint16_t crc = (upper_crc << 8) | lower_crc;
    uint16_t calculated_crc = CRC_Calculate(data_buffer, SUBBLOCK_SIZE, 1, 1);             // Calculate CRC

    if(crc == calculated_crc){                                                      // Check calculated vs received CRC
        crc_subblock_buffer[subblock_number] = crc;                                 // Store CRC of subblock
        subblock_buffer[subblock_number] = 1;                                       // Store flag that represents a good check for this subblock
        uint32_t address = (image_being_stored - 1) * MAX_IMAGE_SIZE + 
                           current_block * BLOCK_SIZE + 
                           subblock_number * SUBBLOCK_SIZE;
        at25df641a_write(address, data_buffer, SUBBLOCK_SIZE);
        at25df641a_read(address, debug_buffer, SUBBLOCK_SIZE);
    }
    else{
        i = 0;
    }
}

/*************************** END BLOCK INTERACTION ********************************/

/* Processes End_Block command
 * Checks CRC of entire block
 *
 * @parameter *data         incoming packet
 */
uint8_t PROCESSMSG_EndBlock(uint8_t *data){
    uint16_t upper_crc = *data++;                                                   // Grab upper CRC value
    uint16_t lower_crc = *data++;                                                   // Grab lower CRC value
    uint16_t crc = (upper_crc << 8) | lower_crc;                                    // Combine into one CRC value

    uint16_t calculated_crc = 0;                                                     // Calcualte CRC of entire block by calculating subblock by subblock

    int initial_run = 1;
    int final_run = 0;
    for(i = 0; i < NUMBER_OF_SUBBLOCKS; i++){   
        if(i == (NUMBER_OF_SUBBLOCKS - 1)){
            final_run = 1;
        }
        uint32_t address = (image_being_stored - 1) * MAX_IMAGE_SIZE + current_block * BLOCK_SIZE + i * SUBBLOCK_SIZE;
        at25df641a_read(address, data_buffer, SUBBLOCK_SIZE);
        calculated_crc = CRC_Calculate(data_buffer, SUBBLOCK_SIZE, initial_run, final_run);
        initial_run = 0;
    }

    uint8_t status = GOOD_BLOCK;                                                     // Used to store status of block
    uint8_t bad_subblocks = 0;                                                       // How many subblocks are bad
    uint8_t bad_subblock_buffer[32];                                                 // Which subblocks are bad

    if(crc == calculated_crc){                                                       // Check calculated vs received CRC
        crc_block_buffer[current_block] = crc;                                       // Store CRC of subblock
        block_buffer[current_block] = 1;                                             // Store flag that represents a good check for this block
    }
    else{
        status = BAD_BLOCK;                                                          // Set status to BAD BLOCK
    }

    if(status == BAD_BLOCK){                                                            
        for(i = 0; i < NUMBER_OF_SUBBLOCKS; i++){                                    // Iterate through subblocks and determine if any are missing
            if(subblock_buffer[i] == 0){
                bad_subblock_buffer[bad_subblocks] = i;
                bad_subblocks++;
            }
        }
        if(bad_subblocks > 0){
            status = MISSING_SUBBLOCKS;                                              // Set status to missing subblocks
        }
    }
    if(status == BAD_BLOCK){                                                         // Iterate through subblocks and check crcs
        for(i = 0; i < NUMBER_OF_SUBBLOCKS; i++){                                   
            uint32_t address = image_being_stored * MAX_IMAGE_SIZE + current_block * BLOCK_SIZE + i * SUBBLOCK_SIZE;
            at25df641a_read(address, data_buffer, SUBBLOCK_SIZE);
            if(crc_subblock_buffer[i] != CRC_Calculate(data_buffer, SUBBLOCK_SIZE, 1, 1)){
                bad_subblock_buffer[bad_subblocks] = i;
                bad_subblocks++;
            }         
        }   
        if(bad_subblocks > 0){
            status = BAD_CRC_SUBBLOCKS;                                                        // Set status to bad CRC
        }
    }
    if(status == BAD_BLOCK){
        status = UNKNOWN_ERROR;                                                            // If neither missing nor bad CRC, set status to unknown error
    }

    SEND_EndBlockResponse(status, bad_subblock_buffer, bad_subblocks);

    if(status == GOOD_BLOCK){
        return SUCCESS;
    }
    else{
        return FAILURE;
    }

}


/* Sends the END BLOCK response. Will only send one byte if block is good.
 * If not sends error status and which subblocks are the problem 
 */
void SEND_EndBlockResponse(uint8_t status, uint8_t* status_buffer, uint8_t status_buffer_size){
    uint8_t length = 0;
    transmit_buffer[0] = END_BLOCK_RESPONSE;

    if(status == GOOD_BLOCK){
        transmit_buffer[1] = GOOD_BLOCK;
        length = 2;
    }
    else{
        transmit_buffer[1] = status;
        length = 2;
        for(i = 0; i < status_buffer_size; i++){
            transmit_buffer[i+1] = *status_buffer++;
            length++;
        }
    }

    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}




/*********************************** END FIRMWARE TRANSACTION *************************************/

/* Processes End_Firmware command
 * Checks CRC of entire Image
 *
 * @parameter *data         incoming packet
 */
uint8_t PROCESSMSG_EndFirmware(uint8_t *data){
    uint8_t upper_firmware_version = *data++;                                              // Grab upper firmware version value
    uint8_t lower_firmware_version = *data++;                                              // Grab lower firmware version value
    uint16_t firmware_version = (0xFFFF & upper_firmware_version << 8) | lower_firmware_version;     // Combines into complete firmware version value

    uint8_t upper_crc = *data++;                                                   // Grab upper CRC value
    uint8_t lower_crc = *data++;                                                   // Grab lower CRC value
    uint16_t crc = (0xFFFF & upper_crc << 8) | lower_crc;                                    // Combines into complete CRC value

    uint16_t calculated_crc = 0;

    int initial_run = 1;
    int final_run = 0;
    for(i = 0; i < NUMBER_OF_SUBBLOCKS * number_of_blocks; i++){                    // Iterates through entire image and calculates CRC
        if(i == (NUMBER_OF_SUBBLOCKS * number_of_blocks - 1)){
            final_run = 1;
        }
        uint32_t address = (image_being_stored - 1) * MAX_IMAGE_SIZE + i * SUBBLOCK_SIZE;
        at25df641a_read(address, data_buffer, SUBBLOCK_SIZE);
        calculated_crc = CRC_Calculate(data_buffer, SUBBLOCK_SIZE, initial_run, final_run); 
        initial_run = 0;
    }

    uint8_t status = GOOD_IMAGE;                                                    // Sets status to good image until changed to bad
    uint8_t bad_blocks = 0;                                                         // Stores number of bad blocks
    uint8_t bad_block_buffer[32];                                                   // Stores which blocks are bad

    if(crc == calculated_crc){                                                      // Check calculated vs received CRC
        uint32_t image_version_address = SYSTEM_INFORMATION_BASE_ADDRESS + (image_being_stored * 2);   // CRC stored in second memory address of allocated image memory
        at25df641a_write(image_crc_address, &lower_firmware_version, 1);
        at25df641a_write(image_crc_address+1, &upper_firmware_version, 1);
    }
    else{
        status = BAD_IMAGE;                                                         // If CRCs dont match, satus is changed to bad image
    }

    if(status == BAD_IMAGE){                                                        // Check for missing blocks
        for(i = 0; i < number_of_blocks; i++){
            if(block_buffer[i] == 0){
                bad_block_buffer[bad_blocks] = i;
                bad_blocks++;
            }
        }
        if(bad_blocks > 0){
            status = MISSING_BLOCKS;
        }
    }
    if(status == BAD_IMAGE){                                                        // Check for bad CRCs
        for(i = 0; i < number_of_blocks; i++){
            calculated_crc = 0;      
            for(j = 0; j < NUMBER_OF_SUBBLOCKS; j++){                                   
                uint32_t address = image_being_stored * MAX_IMAGE_SIZE + i * BLOCK_SIZE + j * SUBBLOCK_SIZE;
                at25df641a_read(address, data_buffer, SUBBLOCK_SIZE);  
                calculated_crc = CRC_Calculate(data_buffer, SUBBLOCK_SIZE, 1, 1);      
            }
            if(crc_block_buffer[i] != calculated_crc){
                bad_block_buffer[bad_blocks] = i;
                bad_blocks++;
            } 
        }   
        if(bad_blocks > 0){
            status = BAD_CRC_BLOCKS;
        }
    }
    if(status == BAD_IMAGE){                                                        // If neither missing bloks or bad CRCs, status is set to unknown error
        status = UNKNOWN_ERROR;
    }

    SEND_EndFirmwareResponse(status, bad_block_buffer, bad_blocks);  

    if(status == GOOD_BLOCK){
        return SUCCESS;
    }
    else{
        return FAILURE;
    }
}

/* Sends the End Firmware response. Will only send one byte if image is good.
 * If not, sends error status and which blocks are the problem 
 */
void SEND_EndFirmwareResponse(uint8_t status, uint8_t* status_buffer, uint8_t status_buffer_size){
    uint8_t length = 0;

    if(status == GOOD_BLOCK){
        transmit_buffer[0] = GOOD_BLOCK;
        length = 1;
    }
    else{
        transmit_buffer[0] = status;
        length = 1;
        for(i = 0; i < status_buffer_size; i++){
            transmit_buffer[i+1] = *status_buffer++;
            length++;
        }
    }

    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}


/***************************** LOAD FIRMWARE TRANSACTION ***************************/

void PROCESSMSG_LoadFirmware(uint8_t *data){
    uint8_t image = *data; 
    //SetBootloadFlag();

    SEND_LoadNewFirmwareResponse(image);
}

void SEND_LoadNewFirmwareResponse(uint8_t image){
    transmit_buffer[0] = LOAD_NEW_FIRMWARE_RESPONSE;
    transmit_buffer[1] = image;
    
    uint8_t length = 2;
    
    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}

void SEND_Statestatus(){
    transmit_buffer[0] = STATE_STATUS;
    transmit_buffer[1] = current_state;
    
    uint8_t length = 2;
    
    BOOTLOADSYSTEM_Transmitter(transmit_buffer, length);
}
