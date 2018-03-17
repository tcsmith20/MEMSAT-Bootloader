/*
 * File:   at25df641a.h
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 *
 * Created on November 15, 2016, 11:34 PM
 */

#ifndef AT25DF641A_H
#define	AT25DF641A_H

//#ifdef	__cplusplus
//extern "C" {
//#endif

#include "stdlib.h"

#define AT25DF641A_READ_ARRAY_100_MHz      0x1B
#define AT25DF641A_READ_ARRAY_85_MHz       0x0B
#define AT25DF641A_READ_ARRAY_40_MHz       0x03
#define AT25DF641A_READ_ARRAY_DUAL_OUT     0x3B
#define AT25DF641A_BLOCK_ERASE_4KB         0x20
#define AT25DF641A_BLOCK_ERASE_32KB        0x52
#define AT25DF641A_BLOCK_ERASE_64KB        0xD8
#define AT25DF641A_CHIP_ERASE              0x60 // there is also 0xC7, not sure the difference
#define AT25DF641A_PAGE_PROGRAM            0x02
#define AT25DF641A_PAGE_PROGRAM_DUAL_IN    0xA2
#define AT25DF641A_PROGRAM_ERASE_SUSPEND   0xB0
#define AT25DF641A_PROGRAM_ERASE_RESUME    0xD0
#define AT25DF641A_WRITE_ENABLE            0x06
#define AT25DF641A_WRITE_DISABLE           0x04
#define AT25DF641A_PROTECT_SECTOR          0x36
#define AT25DF641A_UNPROTECT_SECTOR        0x39
#define AT25DF641A_READ_SECTOR_PROTECT_REG 0x3C
#define AT25DF641A_LOCK_SECTOR             0x33
#define AT25DF641A_FREEZE_SECTOR_LOCK      0x34
#define AT25DF641A_READ_SETOR_LOCK         0x35
#define AT25DF641A_SET_OTP_REG             0x9B
#define AT25DF641A_READ_OTP_REG            0x77
#define AT25DF641A_READ_STATUS             0x05
#define AT25DF641A_WRITE_STATUS_1          0x01
#define AT25DF641A_WRITE_STATUS_2          0x31
#define AT25DF641A_RESET                   0xF0
#define AT25DF641A_READ_ID                 0x9F
#define AT25DF641A_DEEP_POWER_DOWN         0xB9
#define AT25DF641A_POWER_WAKE              0xAB

typedef struct {
    struct __attribute__((__packed__)) {
        /* Bit    0 */ uint8_t deviceIsBusy            : 1;
        /* Bit    1 */ uint8_t eraseSuspendStatus      : 1;
        /* Bit    2 */ uint8_t programSuspendStatus    : 1;
        /* Bit    3 */ uint8_t sectorLockdownEnabled   : 1;
        /* Bit    4 */ uint8_t resetEnabled            : 1;
        /* Bits 5-7 */ uint8_t                         : 3;
    } byte2;
    struct __attribute__((__packed__)) {
        /* Bit    0 */ uint8_t deviceIsBusy            : 1;
        /* Bit    1 */ uint8_t writeEnableLatch        : 1;
        /* Bits 2-3 */ uint8_t protectStatus           : 2;
        /* Bit    4 */ uint8_t writeProtect            : 1;
        /* Bit    5 */ uint8_t errorDetected           : 1;
        /* Bit    6 */ uint8_t /* reserved */          : 1;
        /* Bit    7 */ uint8_t sectorProtectRegsLocked : 1;
    } byte1;
} AT25DF641AStatus;

void at25df641a_enableWrite();
void at25df641a_disableWrite();
void at25df641a_disableProtect(const uint32_t addr);
void at25df641a_bulkErase(const uint32_t addr);
void at25df641a_getStatus(AT25DF641AStatus *const status);
void at25df641a_write(const uint32_t addr, uint8_t const*const data, const size_t numBytes);
void at25df641a_read(const uint32_t addr, uint8_t *const reply, const size_t numBytes);

//#ifdef	__cplusplus
//}
//#endif

#endif	/* AT25DF641A_H */
