/* 
 * File:   crc.c
 * Author: Tanner Smith
 *
 * Created on March 6, 2018, 9:37 PM
 */

#include "xc.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "crc.h"

uint16_t CRC_Calculate(uint8_t* data_buffer, uint8_t size, int initial_run, int final_run){
    static unsigned int Carry,j;
    static unsigned char *ptr;
    static unsigned char Flag = 0;
    ptr=(unsigned char *)&CRCDAT;
    
    CRCCON = 0x0000F;                           // ="sizegth of polynomial-1"
    CRCXOR = 0x1021;                            // generator Polynomial
    if(initial_run){
        CRCWDAT= 0x00;                          // Initialize CRCWDAT with 0
        Flag = 0;
    }
    
    Nop();
    
    for(j=0;j<size;j++){
        *ptr =*data_buffer++;                          //write data into FIFO
        Flag=Flag^0x01;                         //Flag for odd or even bytes
        if(CRCCONbits.CRCFUL==1){               //check if FIFO is full
            CRCCONbits.CRCGO=1;                 //start CRC engine
            while(CRCCONbits.CRCMPT!=1);        //check if FIFO is empty
            CRCCONbits.CRCGO=0;                 //stop CRC engine
        }
    }
    
    if(final_run){
        if(CRCCONbits.CRCGO!=1)
        CRCCONbits.CRCGO=1;

        if(Flag==0)
            CRCDAT = 0x0000;                            //appending PLEN+1 zeros (multiply by 2^16)
        else
            *ptr=0x00;                                  //appending (PLEN+1)/2 zeros (multiply by 2^8)

        while(CRCCONbits.CRCMPT!=1);                //check if FIFO is empty

        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        CRCCONbits.CRCGO=0;                         //stop CRC engine
        Nop();

        if(Flag==1){                                // if odd number of bytes,
            for(j = 0; j < 8; j ++){                     //append (PLEN+1)/2 zeros (multiply by 2^8)
                Carry =( CRCWDAT & 0x8000);
                CRCWDAT <<= 1;
                if(Carry)
                CRCWDAT ^= 0x1021;
            }
        }
    }
    return CRCWDAT;
}