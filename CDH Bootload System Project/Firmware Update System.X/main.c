
/* 
 * File:   main.c
 * Author: nsd
 *
 * Created on November 12, 2016, 10:35 PM
 */

#if defined(__XC16__)
#include <xc.h>
#endif
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"
//#include "m25p05.h"
#include "at25df641a.h"
#include "system.h"
#include "MemSat_SYSTEM.h"
#include "hal_general.h"
#include "uart.h"
#include "bootload_system.h"
#include "crc.h"
#pragma config JTAGEN = OFF
#pragma config IOL1WAY = OFF // Allow us to set/clear IOLOCK as desired

uint8_t receive_array[256];
uint8_t receive_size = 0;

void UART_Transmit(uint8_t data);
void UART_Receive(uint8_t data);

/*
 * 
 */
int16_t main()
{
    
	spi_init();
    
    // Embedded library init
    Timing_Init();
    Task_Init();
	
    UART_Init(UART_CHANNEL);
    UART_RegisterReceiver(UART_CHANNEL, (charReceiver_t)UART_Receive);
    UART_Printf(UART_CHANNEL, "UART Initialized\r\n");
    
    BOOTLOADSYSTEM_Init(UART_Transmit);

   EnableInterrupts();

    while (1) {
        SystemTick();
    }
}

void UART_Transmit(uint8_t data){
    UART_Printf(UART_CHANNEL, "%02x", data);
 }

void UART_Receive(uint8_t data){
    if(data == 202){
        receive_size = 0;
        BOOTLOADSYSTEM_Receiver(receive_array);
        receive_size = 0;
    }
    else{
        receive_array[receive_size] = data;
        receive_size++;
    }

}