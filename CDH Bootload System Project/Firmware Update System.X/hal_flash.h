/* 
 * File:   hal_flash.h
 * Author: Nick Felker
 *
 * Created on March 25, 2017, 6:59 PM
 */

#ifndef HAL_FLASH_H
#define	HAL_FLASH_H

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
} FlashStatus;

typedef struct {
    void (*init)();
    void (*select)();
    void (*deselect)();
    void (*blockingSendByte)(const uint8_t data);
    void (*blockingSendSeq)(const uint8_t* data, const uint8_t len);
    void (*blockingReadSeq)(uint8_t* data, const uint8_t len);
} SPI_IO;

typedef struct {
    void (*enableWrite)(SPI_IO const*const spi);
    void (*disableWrite)(SPI_IO const*const spi);
    void (*disableProtect)(SPI_IO const*const spi, const uint32_t address);
    void (*bulkErase)(SPI_IO const*const spi, const uint32_t addr);
    void (*getStatus)(SPI_IO const*const spi, FlashStatus *const status);
    void (*write)(SPI_IO const*const spi, const uint32_t addr, uint8_t const*const data, const uint8_t numBytes);
    void (*read)(SPI_IO const*const spi, const uint32_t addr, uint8_t *const reply, const uint8_t numBytes);
} Flash;

#endif	/* HAL_FLASH_H */

