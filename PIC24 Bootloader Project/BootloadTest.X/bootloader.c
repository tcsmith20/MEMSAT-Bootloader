/*
 * File:   bootloader.c
 * Author: Tanner Smith
 *
 * Created on February 28, 2018, 12:27 PM
 */


#include "xc.h"
#include "bootloader.h"
#include "at25df641a.h"
#include "progmem.h"

#define PROGRAM_SIZE  0x40000

static uint8_t chip_data[2048];

static int i = 0;
static int j = 0;

void BOOTLOAD_Load(int image);

void BOOTLOAD_Load(int image) {
    uint32_t offset = image * PROGRAM_SIZE;                                     // Set offset to start pointer at image wanting to bootload 
    for(i=offset; i < offset + PROGRAM_SIZE; i += 2048){                        // Loop to go from beginning to end of program
        for(j = 0; j < 2048; j += 128){                                         // Loop to update blocks of memory in chunks of 512 instructions or 2048 bytes
            at25df641a_read(j + i, chip_data + j,128);                          // Read 512 instructions
            if((offset + j) == offset){                                         // Check if beginning of bootload
                chip_data[0] = 0x00;
                chip_data[1] = 0x09;
                chip_data[2] = 0x04;
                chip_data[3] = 0x00;                                            // Hardcode reset vector to point to bootloader
                chip_data[4] = 0x02;
                chip_data[5] = 0x00;
                chip_data[6] = 0x00;
                chip_data[7] = 0x00;
            }
        }
        
        
        
        if((j + i - offset) >= BOOTLOADER_MEMORY_START_ADDRESS){                // If program memory address being written to is in bootloader memory
                                                                                // Don't overwrite the bootloader
        }
        else{
            PROGMEM_UpdateBlock(chip_data, (i-offset)>>1);                      // Update block
        }       
    }
}
