/* @fil MemSat_UART.h
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Function prototypes for the UART module on the PIC24FJ256GA110.
 * @upd 1/24/2017 Changed UART_Write to take both pointer to and size of data
 */

#ifndef MEMSAT_UART_H
#define	MEMSAT_UART_H

/* @fnc MEMSAT_UART_Init()
 * @dsc Sets the necessary configuration registers to use the UART module in the
 * context of MemSat. The UART parameters such as Baud rate, parity, and word length
 * are subject to the needs of the communications beacon.
 */
void MEMSAT_UART_Init();

void MEMSAT_UART_Receiver(unsigned char);

char MEMSAT_UART_Beacon_Status();

void MEMSAT_UART_Beacon_Reset_Status();
#endif
