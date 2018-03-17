/* @fil system.h
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Necessary includes and definitions for use of the Embedded Class Library.
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#define USE_MODULE_TASK
#define USE_MODULE_LIST
#define USE_MODULE_UART
#define USE_MODULE_BUFFER_PRINTF
#define FCPU 4000000
#define UART_BAUD 38400
#define USE_UART1
#define USE_UART2
#define UART_CHANNEL_BEACON UART1_CH
#define UART_CHANNEL UART2_CH
#define UART1_TX_PIN 16
#define UART1_RX_PIN 30
#define UART2_TX_PIN 17 // Package pin 78
#define UART2_RX_PIN 10 // Package pin 68
#define BAUD_RATE_UART1 38400L
#define BAUD_RATE_UART2 38400L
#ifndef tint_t
#define tint_t uint32_t
#endif
#include <xc.h>
#include "list.h"
#include "timing.h"
#include "task.h"
#include "uart.h"
#endif

#define MODE_DEBUG 1
#define MODE_RELEASE 2
#define MODE MODE_DEBUG
#define TRUE 1
#define FALSE 0
#define UART_RESPONSE_BEACON 1
#define UART_RESPONSE_ECHO 2
#define UART_RESPONSE_DECRYPT_ENCRYPT 10
#define UART_RESPONSE_DECRYPT_ENCRYPT_TIMING 20
#define UART_RESPONSE_TIMING_TIMING 21
#define UART_RESPONSE_FLASH_TIMING 22
#define UART_RESPONSE_MOCK_PAYLOAD 30
#define UART_RESPONSE_MOCK_BEACON_FLASH 31
#define UART_RESPONSE_MOCK_COMMS 32

#if MODE == MODE_DEBUG
#define FEATURE_BLINKY TRUE
#define FEATURE_UART_RESPONSE UART_RESPONSE_BEACON
#define FEATURE_TIMING_PROFILES TRUE // In release we won't need unnecessary code
#define AES_KEY "Rowan University" // Must be 16 chars
#define API_KEY "Rowan University" // Must be 16 chars
    #if FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_COMMS || \
        FEATURE_UART_RESPONSE == UART_RESPONSE_DECRYPT_ENCRYPT || \
        FEATURE_UART_RESPONSE == UART_RESPONSE_DECRYPT_ENCRYPT_TIMING || \
        FEATURE_UART_RESPONSE == UART_RESPONSE_TIMING_TIMING || \
        FEATURE_UART_RESPONSE == UART_RESPONSE_FLASH_TIMING
    #define UART_BUFFER TRUE
    #endif
#define TIME_ELAPSED_DEPLOY_ANTENNA 1000 * 60 * 1 // 30 minutes
#define TIME_ELAPSED_START_COMMS 1000 * 60 * 0.5 // 15 minutes after that
#define DEBUG_UART_PRINTF TRUE
#define FLASH_OVERFLOW_OVERWRITE TRUE
#elif MODE == MODE_RELEASE
#define FEATURE_BLINKY FALSE
#define FEATURE_UART_RESPONSE UART_RESPONSE_BEACON
#define FEATURE_TIMING_PROFILES FALSE
#define AES_KEY "RowanMemsat12345" // Must be 16 chars
#define TIME_ELAPSED_DEPLOY_ANTENNA 1000 * 60 * 30 // 30 minutes
#define TIME_ELAPSED_START_COMMS 1000 * 60 * 15 // 15 minutes after that
#endif