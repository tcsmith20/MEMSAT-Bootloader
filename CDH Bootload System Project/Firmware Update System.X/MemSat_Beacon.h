/* 
 * File:   MemSat_Beacon.h
 * Author: Mike
 *
 * Created on February 14, 2017, 2:47 PM
 */

#ifndef MEMSAT_BEACON_H
#define	MEMSAT_BEACON_H

int Beacon_Initialized();

void Beacon_Init(void *);

void MemSat_Beacon_Check_Status();

void Beacon_Setup();

void Beacon_Write(char * buffer, uint8_t buffer_length);

void Beacon_Test();

#if FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_BEACON_FLASH
void MemSat_Beacon_Print_Flash();
#endif
   
#endif	/* MEMSAT_BEACON_H */

