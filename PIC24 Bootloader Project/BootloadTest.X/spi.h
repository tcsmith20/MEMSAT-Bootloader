
/*
 * spi.h
 *
 *  Created on: Oct 29, 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#ifndef SPI_H
#define SPI_H
#include <stdint.h>

void spi_init();
void spi_select();
void spi_deselect();
void spi_blockingSendSeq(uint8_t const* data, const uint8_t len);
void spi_blockingSendByte(const uint8_t data);
void spi_blockingReadSeq(uint8_t *reply, const uint8_t len);

#endif /* SPI_H */