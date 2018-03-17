/*
 * m25p05.h
 *
 *  Created on: 2 Nov 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */

#ifndef M25P05_H_
#define M25P05_H_

#define M25P05_WREN 0b00000110 // 0x06 Write Enable
#define M25P05_WRDI 0b00000100 // 0x04 Write Disable
#define M25P05_RDSR 0b00000101 // 0x05 Read Status Register
#define M25P05_WRSR 0b00000001 // 0x01 Write Status Register
#define M25P05_READ 0b00000011 // 0x03 Read Data Bytes
#define M25P05_PP   0b00000010 // 0x02 Page Program
#define M25P05_SE   0b11011000 // 0xD8 Sector Erase
#define M25P05_BE   0b11000111 // 0xC7 Bulk Erase
#define M25P05_DP   0b10111001 // 0xB9 Deep Power-down
#define M25P05_RES  0b10101011 /* 0xAB Release from deep power down and read
                                       electronic signature */

typedef struct {
	uint8_t writeInProgress            : 1;
	uint8_t writeEnableLatch           : 1;
	uint8_t blockProtect0              : 1;
	uint8_t blockProtect1              : 1;
	uint8_t                            : 3;
	uint8_t statusRegisterWriteDisable : 1;
} M25P05Status;

void m25p05_enableWrite();
void m25p05_disableWrite();
void m25p05_bulkErase();
void m25p05_getStatus(M25P05Status *const status);
void m25p05_write(const uint32_t addr, uint8_t const*const data,
                  const size_t numBytes);
void m25p05_read(const uint32_t addr, uint8_t *const reply,
                 const size_t numBytes);

#endif /* M25P05_H_ */
