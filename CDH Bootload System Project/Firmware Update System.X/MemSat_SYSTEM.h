/* @fil MemSat_UART.h
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Contains all satellite-wide configurations and parameter definitions, and all
 * module includes. This is intentionally removed from system.h in order to limit confusion
 * between the Embedded Class Library and the MemSat API. However, the MemSat API does
 * have a dependency on the Embedded Class Library, and developers should take care to
 * maintain a clean integration between the two systems.
 */

/************************************************************************************************
 * If there are any change to the system clock, go to I2Clib.c recalculate the I2C1BRG (line 68)*
 ************************************************************************************************/

#ifndef MEMSAT_SYSTEM_H
#define	MEMSAT_SYSTEM_H

#define USE_MODULE_TASK
#define USE_MODULE_LIST
#define FCPU 4000000
#define FOSC 8000000
#define FCY (FOSC/2)
#define UART_Baud 38400
#define BAUD_RATE_UART1 38400L
#include <xc.h>
#include "MemSat_SPI.h"
//#include "MemSat_RTCC.h"
#include "MemSat_ADC.h"
#include "MemSat_UART.h"
#include "MemSat_Beacon.h"
#include "MemSat_FLASH.h"
#include "MemSat_Flash_Addr.h"
#include "MemSat_Payload.h"
#include "I2Clib.h"
#include "MemSat_Power.h"
#include "system.h"
#include "at25df641a.h"
#include "spi.h"

#if MODE == MODE_DEBUG    
#include "MemSat_Debug.h"
#endif

void MemSat_Antenna_Deploy();
void MemSat_Antenna_Init_Comms();
void MemSat_System_Init();
void MemSat_8MHZ_Oscillator_Activate();
void MemSat_On_Beacon_Ready();
void MemSat_Watchdog_Disable();


#endif