/* 
 * File:   bootload_system.h
 * Author: Tanner Smith
 * 
 * Created on March 6, 2018, 9:37 PM
 */

#ifndef BOOTLOAD_SYSTEM_H
#define	BOOTLOAD_SYSTEM_H


/********** BOOTLOAD SYSTEM STATES **********/
//#define IDLE                                        0x00
//#define START_FIRMWARE_UPDATE                       0x01
//#define START_OF_BLOCK                              0x02
//#define RECEIVE_DATA                                0x03
//#define END_OF_BLOCK                                0x04
//#define END_OF_FIRMWARE                             0x05
//#define LOAD_NEW_FIRMWARE                           0x06


/********** BOOTLOAD SYSTEM MESSAGES **********/
#define GET_FIRMWARE_VERSION_MSG                    0x00
#define GIVE_FIRMWARE_VERSION_RESPONSE              0x01
#define START_FIRMWARE_UPDATE_MSG                   0x02
#define START_FIRMWARE_UPDATE_RESPONSE              0x03
#define START_BLOCK_MSG                             0x04
#define START_BLOCK_RESPONSE                        0x05
#define RECEIVE_DATA_MSG                            0x06
#define END_BLOCK_MSG                               0x08
#define END_BLOCK_RESPONSE                          0x09
#define END_FIRMWARE_MSG                            0x0A
#define END_FIRMWARE_RESPONSE                       0x0B
#define LOAD_NEW_FIRMWARE_MSG                       0xF0
#define LOAD_NEW_FIRMWARE_RESPONSE                  0xF1
#define STATE_STATUS                                0x55

/********** STATUS MESSAGES **********/
#define SUCCESS                                     0xFF
#define FAILURE                                     0x00
#define READY_TO_UPDATE                             0xFF
#define NOT_READY_TO_UPDATE                         0x00
#define GOOD_BLOCK                                  0xFF
#define BAD_BLOCK                                   0x00
#define GOOD_BLOCK                                  0xFF
#define BAD_BLOCK                                   0x00
#define MISSING_SUBBLOCKS                           0x22
#define BAD_CRC_SUBBLOCKS                           0x44
#define GOOD_IMAGE                                  0xFF
#define BAD_IMAGE                                   0x00
#define MISSING_BLOCKS                              0x22
#define BAD_CRC_BLOCKS                              0x44
#define UNKNOWN_ERROR                               0x88


/********** SIZE DEFINITIONS **********/
#define MAX_IMAGE_SIZE                           0x40000
#define BLOCK_SIZE                                  4096
#define SUBBLOCK_SIZE                                128
#define NUMBER_OF_SUBBLOCKS                           32


/********** SPECIFIC ADDRESSES **********/
#define SYSTEM_INFORMATION_BASE_ADDRESS       0x04000000

/********** FUNCTIONS **********/
void BOOTLOADSYSTEM_Init(void (*transmitter)(uint8_t));
void BOOTLOADSYSTEM_Receiver(uint8_t * data);
void BOOTLOADSYSTEM_Transmitter(uint8_t * data, uint8_t length);

#endif	/* BOOTLOAD_SYSTEM_H */

