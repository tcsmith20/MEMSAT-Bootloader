/* 
 * File:   MemSat_Payload.h
 * Author: Nick Felker
 *
 * Created on March 23, 2017, 8:35 PM
 */

#ifndef MEMSAT_PAYLOAD_H
#define	MEMSAT_PAYLOAD_H
#include <stdint.h>
#include "MemSat_SYSTEM.h"
/*
 * Defines a bit where the memory is permanently latched.
 */
#define MEMORY_LATCHED 0
/*
 * Defines a bit where the memory is temporarily flipped.
 */
#define MEMORY_FLIPPED 1
/**
 * Defines an action to read the number of available samples from the Payload 
 */
#define PAYLOAD_ACTION_COUNT 1
/**
 * Defines an action to read a sample from the Payload
 */
#define PAYLOAD_ACTION_READ 2
/**
 * Defines an action to obtain a status from the payload
 */
#define PAYLOAD_ACTION_STATUS 3

/**
 * Payload request acknowledgment
 */ 
#define PAYLOAD_ACTION_ROLLCALL 4

/**
 * Defines an action to tell the Payload to initialize
 */
#define PAYLOAD_ACTION_INITALIZE 5
/**
 * Defines the size of the payload packet in bytes
 */
#define PAYLOAD_DATA_SIZE 6

/**
 * Defines the size of one payload log entry in bytes
 */
#define PAYLOAD_LOG_ENTRY_SIZE 5

/**
 * How many payload elements we can store in memory before needing to write
 * to flash.
 */
#define PAYLOAD_BUFFER_MAX 64

/**
 * Payload's acknowledgment of a transmitted message.
 */
#define PAYLOAD_QUERY_ACK 1

/**
 * A struct which defines all of the data that is being
 * received from the Payload subsystem and packs it.
 */
typedef struct {
    unsigned chipIdentifier : 5;
    unsigned memoryAddress : 16;
    unsigned latchOrFlipped : 1;
    unsigned temperature   : 12;
    unsigned radiation     : 12;
} payload_packet;

/**
 * This is the function that will be called on whatever data
 * is received from the Payload subsystem. It will read in
 * the array and return a proper structure of the data.
 * 
 * @param data The data that we received.
 * @return A structured dataset.
 */
payload_packet Payload_Process(uint8_t[]);

/**
 * Prepares the payload data and stores it in program flash.
 * 
 */
void Payload_Log();

/**
 * Timestamps and stores a payload status byte into flash.
 */
void Payload_Get_Status();

/**
 * Get the alive/dead status of memory chips
 * @return chip_status bit array.
 */
uint32_t Payload_Chip_Status();

/**
 * Stores time of death of a chip in flash
 * @param chip Chip number of newly dead chip.
 */
void Payload_Log_Chip_Fail(uint8_t chip);

/**
 * Checks the local buffer size.
 * 
 * @return The number of payload data points currently in memory.
 */
uint8_t Payload_Buffer_Length();

/**
 * Gets a value out of the payload buffer at a certain index. Make sure you
 * check the local buffer size with `Payload_Buffer_Length` in order to prevent
 * overreaching.
 * 
 * @param index The payload buffer index you want to index.
 * @return The 6-item array starting at this memory address;
 */
uint8_t * Payload_Buffer_Get(uint8_t index);

/**
 * Sends an SPI command to payload and reads the amount of 
 * packets that are available to read.
 * 
 * @return A count of data to read.
 */
uint32_t Payload_Data_Available();

/**
 * Sends an SPI command to payload to read a single
 * packet of data. This may be called multiple times based
 * on the size of the buffer.
 * 
 * @param readBuffer The array of data to read into.
 */
void Payload_Read();

/**
 * Sends an SPI command to payload to read a packet of status information
 * and confirm whether there are any issues.
 */
void Payload_Send_Init();

/**
 * Passes Payload command to SPI buffer.
 * @param message The message to be sent (defined above).
 * @return Acknowledgment of query.
 */
uint8_t Payload_Query(uint8_t);

/**
 * Checks that Payload system is responding to messages.
 * @return Acknowledgment received TRUE(1) or FALSE(0)
 */
uint8_t Payload_Ready();

#endif	/* MEMSAT_PAYLOAD_H */
