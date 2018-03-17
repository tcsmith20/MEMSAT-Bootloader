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

static uint8_t chip_data[2048];

static int i = 0;

void BOOTLOAD_Load(int image);
void BOOTLOAD_LoadOriginal();
void BOOTLOAD_LoadCurrent();
void BOOTLOAD_LoadNew();

void BOOTLOAD_Load(int image) {
    
    spi_init();
	
	AT25DF641AStatus status;
    
	at25df641a_disableProtect(0x00);	
	at25df641a_bulkErase(0x00);
   
    at25df641a_getStatus(&status);
    
    switch(image){
        case ORIGINAL_IMAGE:
            BOOTLOAD_LoadOriginal();
            break;
        case CURRENT_IMAGE:
            BOOTLOAD_LoadCurrent();
            break;
        case NEW_IMAGE:
            BOOTLOAD_LoadNew();
            break;
        default:
            break;
    }

}

void BOOTLOAD_LoadOriginal(){
    for(i=0; i < 0x3FFFF; i += 2048){
        at25df641a_read(0x0, chip_data, sizeof(chip_data));
        PROGMEM_UpdateBlock(chip_data, i>>1);
    }
}

void BOOTLOAD_LoadCurrent(){
    for(i=0; i < 0x3FFFF; i += 2048){
        at25df641a_read(0x0, chip_data, sizeof(chip_data));
        PROGMEM_UpdateBlock(chip_data, i>>1);
    }
}

void BOOTLOAD_LoadNew(){
    for(i=0; i < 0x3FFFF; i += 2048){
        at25df641a_read(0x0, chip_data, sizeof(chip_data));
        PROGMEM_UpdateBlock(chip_data, i>>1);
    }
}
