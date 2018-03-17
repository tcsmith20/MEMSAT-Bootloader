/**
 * @defgroup uart UART Module
 * @file uart.h
 *
 *  Created on: Mar 12, 2014
 *      Author: Michael Muhlbaier
 *  Updated on: Feb 7, 2015
 *      Author: Anthony Merlino
 * @{
 */

#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

// Get library configuration
#include "system.h"

#ifndef USE_MODULE_UART
#warning "USE_MODULE_UART not defined in system.h. Other modules won't be able to utilize this module."
#endif

#include "subsys.h"

#ifdef USE_MODULE_TASK
#include "task.h"
#endif

#ifdef USE_MODULE_BUFFER_PRINTF
#include "buffer_printf.h"
#endif

#include "buffer.h"

#include "char_receiver_list.h"

/** NOTE:  This enumeration has to occur BEFORE the include for
 * the hal_uart.h because hal_uart.h includes this uart.h file
 * so that it can see this error enumeration.
 */
enum uart_errors {
    BREAK_ERROR = 0,
    PARITY_ERROR,
    FRAMING_ERROR,
    OVERRUN_ERROR,
    NUM_UART_ERRORS
};

/**************************************
 * Hardware Abstraction Layer Includes
 *************************************/
#include "hal_uart.h"

/** Initialize UART module
 *
 * Example usage:
 * @code
 * UART_Init(UART0_CHANNEL);
 * @endcode
 *
 * @param channel - The channel of UART to be used.  Macros for these should be defined in the
 * HAL of the specific device.
 */
void UART_Init(uint8_t channel);

void UART_ReconfigureBaud(uint8_t channel, uint32_t baud);

void UART_WriteByte(uint8_t channel, char c);

/**
 * @brief Write chunk of data to UART tx buffer
 *
 * @param channel - the UART channel to send on
 * @param data - a pointer to the data to send
 * @param length - the amount of data to send
 * @return 0 = success, -1 = not enough room in buffer, -2 = Invalid channel
 */
int8_t UART_Write(uint8_t channel, char * data, uint16_t length);

// this module could be used without buffer_printf if these functions are
// not used. Flag added to make it easy to not require buffer_printf
#ifdef _BUFFER_PRINTF_H_
void UART_Printf(uint8_t channel, char * str,...);
void UART_vprintf(uint8_t channel, char * str, va_list vars);
#endif

uint8_t UART_IsTransmitting(uint8_t channel);

void UART_Tick(void);
int8_t UART_RegisterReceiver(uint8_t channel, charReceiver_t fn);
void UART_UnregisterReceiver(uint8_t channel, charReceiver_t fn);

void UART_RegisterErrorCallback(uint8_t channel, void(*callback)(enum uart_errors));

void UART_RegisterTxOverwriteCallback(uint8_t channel, void(*overwriteCallback)(void));

void UART_RegisterRxOverwriteCallback(uint8_t channel, void(*overwriteCallback)(void));


/**************************************
 * Hardware Abstraction Layer Declarations
 *************************************/

#ifndef hal_UART_Init
void hal_UART_Init(uint8_t channel, uint32_t baud);
#endif

#ifndef hal_UART_ReconfigureBaud
void hal_UART_ReconfigureBaud(uint8_t channel, uint32_t baud);
#endif

#ifndef UART_Tx_Handler
void UART_Tx_Handler(uint8_t channel);
#endif

#ifndef UART_Rx_Handler
void UART_Rx_Handler(uint8_t channel);
#endif

#ifndef UART_Error_Handler
void UART_Error_Handler(uint8_t channel, enum uart_errors error);
#endif

#ifndef hal_UART_Enable
void hal_UART_Enable(uint8_t channel);
#endif

#ifndef hal_UART_Disable
void hal_UART_Disable(uint8_t channel);
#endif

#ifndef hal_UART_SpaceAvailable
uint8_t hal_UART_SpaceAvailable(uint8_t channel);
#endif

#ifndef hal_UART_DataAvailable
uint8_t hal_UART_DataAvailable(uint8_t channel);
#endif

#ifndef hal_UART_ClearRxIF
void hal_UART_ClearRxIF(uint8_t channel);
#endif

#ifndef hal_UART_ClearTxIF
void hal_UART_ClearTxIF(uint8_t channel);
#endif

#ifndef hal_UART_EnableRxInterrupt
void hal_UART_EnableRxInterrupt(uint8_t channel);
#endif

#ifndef hal_UART_EnableTxInterrupt
void hal_UART_EnableTxInterrupt(uint8_t channel);
#endif

#ifndef hal_UART_DisableRxInterrupt
void hal_UART_DisableRxInterrupt(uint8_t channel);
#endif

#ifndef hal_UART_DisableTxInterrupt
void hal_UART_DisableTxInterrupt(uint8_t channel);
#endif

#ifndef hal_UART_RxInterruptEnabled
uint8_t hal_UART_RxInterruptEnabled(uint8_t channel);
#endif

#ifndef hal_UART_TxInterruptEnabled
uint8_t hal_UART_TxInterruptEnabled(uint8_t channel);
#endif

#ifndef hal_UART_RxChar
char hal_UART_RxChar(uint8_t channel);
#endif

#ifndef hal_UART_TxChar
void hal_UART_TxChar(uint8_t channel, char c);
#endif

/** @}*/
#endif /* _UART_H_ */
