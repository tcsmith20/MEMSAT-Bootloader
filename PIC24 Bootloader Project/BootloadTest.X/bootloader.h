/*
 * File:   bootload.h
 *      Author: Tanner Smith 
 *
 * Created on March 5, 2018, 11:34 AM
 */

#ifndef BOOTLOAD_H
#define	BOOTLOAD_H

#include <xc.h> 

#define BOOTLOADER_MEMORY_START_ADDRESS     0x00

void BOOTLOAD_Load(int image);

#endif	/* BOOTLOAD_H */

