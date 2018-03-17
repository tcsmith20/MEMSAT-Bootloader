/* 
 * File:   MemSat_FLASH.h
 * Author: Mike
 *
 * Created on December 12, 2016, 2:47 PM
 */

#ifndef MEMSAT_FLASH_H
#define	MEMSAT_FLASH_H
#include <stdint.h>
#include "at25df641a.h"
#include "spi.h"
#include "hal_flash.h"

#define FLASH_CHIP (&at25df641a)
#define FLASH_IO (&spi_flash)

static const SPI_IO spi_flash = {
    spi_init,
    spi_select,
    spi_deselect,
    spi_blockingSendByte,
    spi_blockingSendSeq,
    spi_blockingReadSeq
};

static const Flash at25df641a = {
    at25df641a_enableWrite,
    at25df641a_disableWrite,
    at25df641a_disableProtect,
    at25df641a_bulkErase,
    at25df641a_getStatus,
    at25df641a_write,
    at25df641a_read
};

void FLASH_Init(Flash const*const flash, SPI_IO const*const spi);
FlashStatus FLASH_Status(Flash const*const flash, SPI_IO const*const spi);
void FLASH_Write(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes);
void FLASH_Read(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, uint8_t *const reply, const uint8_t numBytes);
void FLASH_Bulk_Erase(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr);
void FLASH_Write_To(Flash const*const flashChip, SPI_IO const*const spi, const uint32_t addr, const uint8_t * data, const uint8_t numBytes);

void Payload_Flash_Write(const Flash* const flashChip, const SPI_IO* const spi, const uint32_t addr, const uint8_t* data, const uint8_t numBytes);
void Payload_FLASH_Push(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t * data);
uint8_t* Payload_FLASH_Pop(Flash const*const flashChip, SPI_IO const*const spi);
void FLASH_Logs_Push(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t * data, const uint8_t numBytes);
uint8_t* FLASH_Logs_Pop(Flash const*const flashChip, SPI_IO const*const spi, const uint8_t numBytes);
void FLASH_System_Launched(Flash const*const flashChip, SPI_IO const*const spi);

#endif	/* MEMSAT_FLASH_H */
