/*
 * m25p05.c
 *
 *  Created on: 2 Nov 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#include <stdint.h>
#include <stdlib.h>
#include "spi.h"
#include "m25p05.h"

void m25p05_enableWrite()
{
	spi_select();
	spi_blockingSendByte(M25P05_WREN);
	spi_deselect();
}

void m25p05_disableWrite()
{
	spi_select();
	spi_blockingSendByte(M25P05_WRDI);
	spi_deselect();
}

void m25p05_write(const uint32_t addr, uint8_t const*const data,
                  const size_t numBytes)
{
	M25P05Status status;

	uint8_t write[] = {
		M25P05_PP,
		(addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
	};

	m25p05_enableWrite();

	spi_select();
	spi_blockingSendSeq(write, sizeof(write));
	spi_blockingSendSeq(data, numBytes);
	spi_deselect();

	while (1) {
		m25p05_getStatus(&status);
		if (!status.writeInProgress) break;
		//__delay_cycles(1000);
	}
}

void m25p05_read(const uint32_t addr, uint8_t *const reply,
                 const size_t numBytes)
{
	uint8_t read[] = {
		M25P05_READ,
		(addr & 0xFF0000) >> 16,
		(addr & 0x00FF00) >>  8,
		(addr & 0x0000FF)
	};
	spi_select();
	spi_blockingSendSeq(read, sizeof(read));

	spi_blockingReadSeq(reply, numBytes);
	spi_deselect();
}

void m25p05_bulkErase()
{
	M25P05Status status;

	m25p05_enableWrite();

	spi_select();
	spi_blockingSendByte(M25P05_BE);
	spi_deselect();

	while (1) {
		m25p05_getStatus(&status);
		if (!status.writeInProgress) break;
		//__delay_cycles(1000);
	}
}

void m25p05_getStatus(M25P05Status *const status)
{
	spi_select();
	spi_blockingSendByte(M25P05_RDSR);
	spi_blockingReadSeq((uint8_t*) status, 1);
	spi_deselect();
}
