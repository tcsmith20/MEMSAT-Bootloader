#ifndef PROGMEM_H
#define	PROGMEM_H

#include "xc.h"
#include <stdint.h>



/***** NVM CONTROL REGISTER (NVMCON) BITS AND FUNCTIONS *****/

#define WRITE_CONTROL_FLAG          0x8000  // 1 - Initiates memory program or memory erase (SETTABLE ONLY)
                                            // 0 - Set by hardware when operation is done
#define WRITE_ENABLE_BIT            0x4000  // 1 - Enables program/erase operations
                                            // 0 - Disables program/erase operations
#define WRITE_SEQUENCE_ERROR_FLAG   0x2000  // 1 - Improper program attempt or termination
                                            // 0 - Normal execution
#define ERASE_BIT                   0x0040  // 1 - Enable erase operation 
                                            // 0 - Enable write operation
#define PAGE_ERASE                  0x0002  // ERASE flag needs to be high
#define WORD_PROGRAM                0x0003  // ERASE flag needs to be low
#define ROW_PROGRAM                 0x0001  // ERASE flag needs to be low

/************************************************************/

#define NUM_INSTRUCTION_PER_ROW         64  

#define SAFE_ADDRESS_START              0x200       // Start of block that will necer be overwritten
#define SAFE_ADDRESS_END                0x7FF       // End of block that will never be overwritten

void PROGMEM_UpdateBlock(uint8_t *mem_data, uint32_t address);
uint8_t* PROGMEM_ReadProgramMemory(uint16_t base_address);
void PROGMEM_LoadNewMemory(uint8_t *mem_data);
void PROGMEM_EraseBlock(unsigned int base_address);
void PROGMEM_WriteBlock(uint16_t base_address);
uint32_t PROGMEM_Read(uint32_t address);

#endif	/* PROGMEM_H */