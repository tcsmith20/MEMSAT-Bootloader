/* 
 * File:   main.c
 * Author: Tanner Smith
 *
 * Created on February 28, 2018, 12:11 PM
 */

#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include "spi.h"
#include "at25df641a.h"
#include "progmem.h"
#include "bootloader.h"


#define BOOTLOADER_FLAG_LOCATION          0x04000008
#define BOOTLOADER_IMAGE_LOCATION         0x04000009
#define BOOTLOADER_FLAG_KEY                     0x13

static int i = 0;

void JumpToApp();

int16_t main(){
    
    spi_init();                                                                 // Initialize SPI
	at25df641a_disableProtect(0x00);	
    
    uint8_t ready = 0;
    at25df641a_read(BOOTLOADER_FLAG_LOCATION, &ready , 1);                      // Check to see if bootload flag is set high
    
    if(ready == BOOTLOADER_FLAG_KEY){
        at25df641a_write(BOOTLOADER_FLAG_LOCATION, 0 , 1);                      // Clear flag if high
        
        uint8_t image = 0;
        at25df641a_read(BOOTLOADER_IMAGE_LOCATION, &image , 1);                 // Get image that will be bootloaded
        BOOTLOAD_Load(image);                                                   // Bootload image
        
        JumpToApp();                                                            // Jump to application
    }
    else{
        JumpToApp();                                                            // Jump to application
    }

	while (1) {}
}

void JumpToApp(){
    uint16_t *address = 0x200;
    goto *address;
}
