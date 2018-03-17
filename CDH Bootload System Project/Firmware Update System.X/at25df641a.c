///*
// * File:   at25df641a.c
// *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
// *                               <deciccon0@students.rowan.edu>
// *
// * Created on November 15, 2016
// */
//
//#include <stdint.h>
//#include <stdlib.h>
//#include "spi.h"
//#include "hal_flash.h"
//#include "at25df641a.h"
//
//void at25df641a_enableWrite(SPI_IO const*const spi)
//{
//	spi->select();
//	spi->blockingSendByte(AT25DF641A_WRITE_ENABLE);
//	spi->deselect();
//}
//
//void at25df641a_disableWrite(SPI_IO const*const spi)
//{
//	spi->select();
//	spi->blockingSendByte(AT25DF641A_WRITE_DISABLE);
//	spi->deselect();
//}
//
//void at25df641a_disableProtect(SPI_IO const*const spi, const uint32_t addr)
//{
//	uint8_t unprotect[] = {
//		AT25DF641A_UNPROTECT_SECTOR,
//		(addr & 0xFF0000) >> 16,
//		(addr & 0x00FF00) >>  8,
//		(addr & 0x0000FF)
//	};
//
//	spi->select();
//	spi->blockingSendSeq(unprotect, sizeof(unprotect));
//	spi->deselect();
//}
//
//void at25df641a_write(SPI_IO const*const spi, 
//        const uint32_t addr, uint8_t const*const data, const uint8_t numBytes)
//{
//	AT25DF641AStatus status;
//
//	uint8_t write[] = {
//		AT25DF641A_PAGE_PROGRAM,
//		(addr & 0xFF0000) >> 16,
//		(addr & 0x00FF00) >>  8,
//		(addr & 0x0000FF)
//	};
//
//	at25df641a_enableWrite(spi);
//
//	spi->select();
//	spi->blockingSendSeq(write, sizeof(write));
//	spi->blockingSendSeq(data, numBytes);
//	spi->deselect();
//
//	while (1) {
//		at25df641a_getStatus(spi, &status);
//		if (!status.byte1.deviceIsBusy) break;
//		//__delay_cycles(1000);
//	}
//}
//
//void at25df641a_read(SPI_IO const*const spi,
//        const uint32_t addr, uint8_t *const reply, const uint8_t numBytes)
//{
//	uint8_t read[] = {
//		AT25DF641A_READ_ARRAY_40_MHz,
//		(addr & 0xFF0000) >> 16,
//		(addr & 0x00FF00) >>  8,
//		(addr & 0x0000FF)
//	};
//	spi->select();
//	spi->blockingSendSeq(read, sizeof(read));
//
//	spi->blockingReadSeq(reply, numBytes);
//	spi->deselect();
//}
//
//void at25df641a_bulkErase(SPI_IO const*const spi, const uint32_t addr)
//{
//	AT25DF641AStatus status;
//    
//    uint8_t erase[] = {
//        AT25DF641A_BLOCK_ERASE_4KB,
//        (addr & 0xFF0000) >> 16,
//		(addr & 0x00FF00) >>  8,
//		(addr & 0x0000FF)
//    };
//
//    at25df641a_enableWrite(spi);
//	spi->select();
//	spi->blockingSendSeq(erase, sizeof(erase));
//	spi->deselect();
//
//	while (1) {
//		at25df641a_getStatus(spi, &status);
//		if (!status.byte1.deviceIsBusy) break;
//		//__delay_cycles(1000);
//	}
//}
//
//void at25df641a_getStatus(SPI_IO const*const spi, AT25DF641AStatus *const status)
//{
//	spi->select();
//	spi->blockingSendByte(AT25DF641A_READ_STATUS);
//	spi->blockingReadSeq((uint8_t*) status, sizeof(AT25DF641AStatus));
//	spi->deselect();
//}


/*
 * File:   at25df641a.c
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 *
 * Created on November 15, 2016
 */

#include <stdint.h>
#include <stdlib.h>
#include "spi.h"
#include "at25df641a.h"

void at25df641a_enableWrite()
{
	spi_select();
	spi_blockingSendByte(AT25DF641A_WRITE_ENABLE);
	spi_deselect();
}

void at25df641a_disableWrite()
{
	spi_select();
	spi_blockingSendByte(AT25DF641A_WRITE_DISABLE);
	spi_deselect();
}

void at25df641a_disableProtect(const uint32_t addr)
{
	uint8_t unprotect[] = {
		AT25DF641A_UNPROTECT_SECTOR,
		(addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
	};

	spi_select();
	spi_blockingSendSeq(unprotect, sizeof(unprotect));
	spi_deselect();
}

void at25df641a_write(const uint32_t addr, uint8_t const*const data,
                      const size_t numBytes)
{
	AT25DF641AStatus status;

	uint8_t write[] = {
		AT25DF641A_PAGE_PROGRAM,
		(addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
	};

	at25df641a_enableWrite();

	spi_select();
	spi_blockingSendSeq(write, sizeof(write));
	spi_blockingSendSeq(data, numBytes);
	spi_deselect();

	while (1) {
		at25df641a_getStatus(&status);
		if (!status.byte1.deviceIsBusy) break;
		//__delay_cycles(1000);
	}
}

void at25df641a_read(const uint32_t addr, uint8_t *const reply,
                     const size_t numBytes)
{
	uint8_t read[] = {
		AT25DF641A_READ_ARRAY_40_MHz,
		(addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
	};
	spi_select();
	spi_blockingSendSeq(read, sizeof(read));

	spi_blockingReadSeq(reply, numBytes);
	spi_deselect();
}

void at25df641a_bulkErase(const uint32_t addr)
{
	AT25DF641AStatus status;
    
    uint8_t erase[] = {
        AT25DF641A_BLOCK_ERASE_64KB,
        (addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
    };

	at25df641a_enableWrite();

	spi_select();
    spi_blockingSendSeq(erase, sizeof(erase)); 
//	spi_blockingSendByte(AT25DF641A_BLOCK_ERASE_64KB);
	spi_deselect();

	while (1) {
		at25df641a_getStatus(&status);
		if (!status.byte1.deviceIsBusy) break;
		//__delay_cycles(1000);
	}
}

void at25df641a_getStatus(AT25DF641AStatus *const status)
{
	spi_select();
	spi_blockingSendByte(AT25DF641A_READ_STATUS);
	spi_blockingReadSeq((uint8_t*) status, sizeof(AT25DF641AStatus));
	spi_deselect();
}
