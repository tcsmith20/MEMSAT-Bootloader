#include "MemSat_FLASH.h"
#include "MemSat_Flash_Addr.h" // Import our address headers
#include "at25df641a.h"
#include "spi.h"
#include "MemSat_Payload.h"
#include "system.h"
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>

#define CS _RD13

/*
 *  Our Flash memory is set up in 4KB sectors. We need to clear a sector before
 *    we can update a value. Therefore, we need a 4KB long array to read in 
 *    all values, change one, and then rewrite all those values back :felker:
 */
#define FLASH_SECTOR_LENGTH 4096 // 4 KB
#define FLASH_PAGE_LENGTH 256 // 256 Bytes
#define FLASH_PAGES_PER_SECTOR FLASH_SECTOR_LENGTH / FLASH_PAGE_LENGTH

uint8_t flash_cache[FLASH_SECTOR_LENGTH];
uint8_t data[PAYLOAD_DATA_SIZE];

void FLASH_Init(Flash const*const flashChip, SPI_IO const*const spi) {
    spi->init();
    TRISD |= 1 << 12; // Chip Select Pin   
    FlashStatus status;
    flashChip->enableWrite(spi);
    flashChip->getStatus(spi, &status);
    flashChip->disableProtect(spi, 0x00);
    flashChip->getStatus(spi, &status);
}
// TODO We need some sort of Flash initializer, at least for our C&DH variables

FlashStatus FLASH_Status(Flash const*const flashChip, SPI_IO const*const spi) {
    FlashStatus status;
    flashChip->getStatus(spi, &status);
    return status;
}

void Payload_Flash_Write(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes) {
    if(addr >= FLASH_PAYLOAD_PRIMARY_DATA && addr < FLASH_SYSTEM_LOGS){
        FLASH_Write(flashChip, spi, addr, data, numBytes);
    }
}

void Payload_FLASH_Push(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t * data) {
    // Use our pointers to determine where to write
    // Our end pointer is to null.
    uint8_t* start = 0;
    uint8_t* end = NULL;
    
    FLASH_Read(flashChip, spi, FLASH_CDH_PAYLOAD_START, start, 1);
    FLASH_Read(flashChip, spi, FLASH_CDH_PAYLOAD_END, end, 1);
    
    if ((uint16_t)end + PAYLOAD_DATA_SIZE > FLASH_PAYLOAD_PRIMARY_DATA + FLASH_CAPACITY_FLASH) {
        end = (uint8_t*)FLASH_PAYLOAD_PRIMARY_DATA;
    }
    if ((uint16_t)start - (uint16_t)end < PAYLOAD_DATA_SIZE && (uint16_t)start - (uint16_t)end > 0) {
#if FLASH_OVERFLOW_OVERWRITE
        // Don't do anything
#else
        // TODO Throw error
        return;
#endif
    }
    // Push payload data
    FLASH_Write_To(flashChip, spi, (uint16_t)end, data, PAYLOAD_DATA_SIZE);
    // Increment our end payload
    FLASH_Write_To(flashChip, spi, FLASH_CDH_PAYLOAD_END, end + PAYLOAD_DATA_SIZE, 1);
}

uint8_t* Payload_FLASH_Pop(Flash const*const flashChip, SPI_IO const*const spi) {
    // Use our pointers to determine where to start reading
    uint8_t* start = 0;
    uint8_t* end = 0;
    FLASH_Read(flashChip, spi, FLASH_CDH_PAYLOAD_START, start, 1);
    FLASH_Read(flashChip, spi, FLASH_CDH_PAYLOAD_END, end, 1);
        
    // Read payload data
    FLASH_Read(flashChip, spi, (uint16_t)start, data, PAYLOAD_DATA_SIZE);
    
    if ((uint16_t)start + PAYLOAD_DATA_SIZE > FLASH_PAYLOAD_PRIMARY_DATA + FLASH_CAPACITY_FLASH) {
        // Wrap around
        start = (uint8_t*)FLASH_PAYLOAD_PRIMARY_DATA;
        if (end - start < PAYLOAD_DATA_SIZE && end - start > 0) {
#if FLASH_OVERFLOW_OVERWRITE
            // Don't do anything
#else
            // TODO Throw error
            return;
#endif
        }
    }

    // Increment our pointer
    FLASH_Write_To(flashChip, spi, FLASH_CDH_PAYLOAD_START, start + PAYLOAD_DATA_SIZE, 1);
    return data;
}

void FLASH_Logs_Push(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t * data, const uint8_t numBytes) {
    // Use our pointers to determine where to write
    // Our end pointer is to null.
    uint8_t* start = 0;
    uint8_t* end = NULL;
    FLASH_Read(flashChip, spi, FLASH_CDH_LOGS_START, start, 1);
    FLASH_Read(flashChip, spi, FLASH_CDH_LOGS_END, end, 1);
    
    if ((uint16_t)end + numBytes > FLASH_SYSTEM_LOGS + FLASH_CAPACITY_LOGS) {
        end = (uint8_t*)FLASH_SYSTEM_LOGS;
    }
    if (start - end < numBytes && start - end > 0) {
#if FLASH_OVERFLOW_OVERWRITE
        // Don't do anything
#else
        // TODO Throw error
        return;
#endif
    }
    
    // Push payload data
    FLASH_Write_To(flashChip, spi, (uint16_t)end, data, numBytes);
    // Increment our end payload
    FLASH_Write_To(flashChip, spi, FLASH_CDH_LOGS_END, end + numBytes, 1);
}

uint8_t* FLASH_Logs_Pop(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t numBytes) {
    // Use our pointers to determine where to start reading
    uint8_t* start = 0;
    uint8_t* end = 0;
    FLASH_Read(flashChip, spi, FLASH_CDH_LOGS_START, start, 1);
    FLASH_Read(flashChip, spi, FLASH_CDH_LOGS_END, end, 1);
    
    // Read payload data
    FLASH_Read(flashChip, spi, (uint16_t)start, data, numBytes);
    
    if ((uint16_t)start + numBytes > FLASH_SYSTEM_LOGS + FLASH_CAPACITY_LOGS) {
        // Wrap around
        start = (uint8_t*)FLASH_SYSTEM_LOGS;
        if (end - start < numBytes && end - start > 0) {
#if FLASH_OVERFLOW_OVERWRITE
            // Don't do anything
#else
            // TODO Throw error
            return;
#endif
        }
    }
    
    // Increment our pointer
    FLASH_Write_To(flashChip, spi, FLASH_CDH_LOGS_START, start + numBytes, 1);
    return data;
}

void FLASH_System_Launched(Flash const*const flashChip, SPI_IO const*const spi) {
    // Flip boolean for system launched var
    FLASH_Write_To(flashChip, spi, FLASH_CDH_SYSTEM_LAUNCHED, (uint8_t*)0x01, 1);
    FLASH_Write_To(flashChip, spi, FLASH_CDH_PAYLOAD_START, (uint8_t*)FLASH_PAYLOAD_PRIMARY_DATA, 1);
    FLASH_Write_To(flashChip, spi, FLASH_CDH_PAYLOAD_END, (uint8_t*)FLASH_PAYLOAD_PRIMARY_DATA + 1, 1);
    FLASH_Write_To(flashChip, spi, FLASH_CDH_LOGS_START, (uint8_t*)FLASH_SYSTEM_LOGS, 1);
    FLASH_Write_To(flashChip, spi, FLASH_CDH_LOGS_END, (uint8_t*)FLASH_SYSTEM_LOGS + 1, 1);
}

void Comms_Flash_Write(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes) {
    if(addr >= FLASH_SYSTEM_LOGS && addr < FLASH_TOTAL_CAPACITY){
        FLASH_Write(flashChip, spi, addr, data, numBytes);
    }
}

void CDH_Flash_Write(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes) {
    if(addr >= FLASH_CDH_CONFIG && addr < FLASH_CDH_CONFIG){
        FLASH_Write(flashChip, spi, addr, data, numBytes);
    }
}

void FLASH_Write(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes)
{
	flashChip->write(spi, addr, data, numBytes);
}

void FLASH_Read(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, uint8_t *const reply, const uint8_t numBytes)
{
    flashChip->read(spi, addr, reply, numBytes);
}

void FLASH_Bulk_Erase(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr)
{
    flashChip->bulkErase(spi, addr);
}

void FLASH_Write_To(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes) {
    // First, we need to grab the address segment
    uint32_t startPtr = addr & 0xFFFFF000; // Get the beginning 4KB sector
    // Read all of those bytes into our flash cache
    FLASH_Read(flashChip, spi, startPtr, flash_cache, (uint8_t)FLASH_SECTOR_LENGTH);
    // Erase 4KB sector
    FLASH_Bulk_Erase(flashChip, spi, startPtr);
    // Now we can change our data
    uint8_t i = 0;
    for (i = 0; i < numBytes; i++) {
        uint16_t dataIndex = addr - startPtr;
        flash_cache[dataIndex] = data[i];
    }
    // Write all of these back to Flash.
    // But we can only write in 256-byte pages.
    for (i = 0; i < FLASH_PAGES_PER_SECTOR; i++) {
        uint16_t offset = i * FLASH_PAGE_LENGTH;
        FLASH_Write(flashChip, spi, startPtr + offset, flash_cache + offset, (uint8_t)FLASH_PAGE_LENGTH);  
    }
    // Okay.
}