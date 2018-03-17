/* 
 * File:   newmain.c
 * Author: tomas
 *
 * Created on February 20, 2018, 9:25 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "xc.h"
#include "bootloader.h"
//#if defined (__PIC24FJ256GB110__)	//Defined by MPLAB when using 24FJ256GB110 device
//    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2) 
//    _CONFIG2( IESO_OFF & FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & PLLDIV_DIV2 & IOL1WAY_ON)
//    _CONFIG3( WPCFG_WPCFGDIS & WPDIS_WPDIS)		//Disable erase/write protect of all memory regions.
//#elif defined (__PIC24FJ256GA110__)	//Defined by MPLAB when using 24FJ256GA110 device
//    _CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF & COE_OFF & FWDTEN_OFF & ICS_PGx2 ) 
//    _CONFIG2( FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMOD_HS & FNOSC_PRIPLL & IOL1WAY_ON)
//    _CONFIG3( WPCFG_WPCFGDIS & WPDIS_WPDIS)		//Disable erase/write protect of all memory regions.
//#else
//	#error This code example does not support this device.
//#endif
/*
 * 
 */

//static uint8_t mem_block[2048];
//static int i = 0;

int main(void) {
    RCONbits.SWDTEN = 0;
    TestProgram3();
    //ButtonTest();
//    
//    uint8_t* data = PROGMEM_ReadProgramMemory(0x00);
//    
//    for(i = 0; i < 2048; i++){
//        mem_block[i] = *data++;
//    }
    
    while(1);
}

