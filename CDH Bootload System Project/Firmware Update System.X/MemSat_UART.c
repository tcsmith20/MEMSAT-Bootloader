/* @fil MemSat_UART.h
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Function implementations for the UART module on the PIC24FJ256GA110.
 */

#include <string.h>
#include <xc.h>
#include "MemSat_SYSTEM.h"
#include "MemSat_UART.h"
#include "MemSat_Comms_Handler.h"
#include "MemSat_Timing_Profile.h"

#define UART_RECEIVE_MAX_LENGTH 256 // Receives up to 256 bytes

#if UART_BUFFER == TRUE
unsigned char MEMSAT_UART_Rx[UART_RECEIVE_MAX_LENGTH];
unsigned int uart_size = 0;
unsigned int uart_desired_length = 0;
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_PAYLOAD
#include "MemSat_Payload.h"
unsigned char payload_rx[6];
unsigned char payload_samples = 0;
unsigned char payload_action = 0;
unsigned char payload_rx_length = 0;
uint32_t uart_chars_received = 0;

void start_test() {
    payload_action = PAYLOAD_ACTION_INITALIZE;
    UART_WriteByte(UART_CHANNEL, PAYLOAD_ACTION_INITALIZE);
}
#endif

unsigned char BEACON_STATE = 0;

void MEMSAT_UART_Init() {
    UART_Init(UART_CHANNEL_BEACON);
    UART_RegisterReceiver(UART_CHANNEL_BEACON, (charReceiver_t) MEMSAT_UART_Receiver);
#if FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_PAYLOAD
    // Need to initialize our reading as we are the master
    Task_Schedule(start_test, 0, 1000, 0);
#endif
}

char MEMSAT_UART_Beacon_Status() {
    return BEACON_STATE;
}

void MEMSAT_UART_Receiver(unsigned char c) {
    BEACON_STATE = c;
#if FEATURE_UART_RESPONSE == UART_RESPONSE_ECHO
    UART_WriteByte(UART_CHANNEL, c);
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_BEACON || FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_BEACON_FLASH
    if (c == 'K') {
        // Ping the Beacon to update
        Beacon_Setup();
    }
#elif UART_BUFFER == TRUE
    if ((c == 254 || c == 0) && uart_size == 0 && uart_desired_length == 0) {
        // Ignore. Some weird artifact.
    } else if (uart_size == 0 && uart_desired_length == 0) {
        uart_desired_length = c + 1; // Sending n+1 bytes in message
    } else {
        MEMSAT_UART_Rx[uart_size++] = c;
        if (uart_size == uart_desired_length) {
#if FEATURE_UART_RESPONSE == UART_RESPONSE_DECRYPT_ENCRYPT
            aes_decrypt(&MEMSAT_UART_Rx, API_KEY, AES_KEY);
            // Modify first character
            MEMSAT_UART_Rx[0] = MEMSAT_UART_Rx[0] + 1;
            // Re-encrypt and write out
            aes_encrypt(&MEMSAT_UART_Rx, &MEMSAT_UART_Rx, API_KEY, AES_KEY);
            int i = 0;
            for (i = 0; i < uart_size; i++) {
                UART_WriteByte(UART_CHANNEL, MEMSAT_UART_Rx[i]);
            }
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_DECRYPT_ENCRYPT_TIMING
            MemSat_Timing_Profile_Start();
            // Encrypt 10 times
            char i = 0;
            for (i = 0; i < 10; i++) {
                aes_encrypt(&MEMSAT_UART_Rx, &MEMSAT_UART_Rx, API_KEY, AES_KEY);
            }
            MemSat_Timing_Profile_Report(UART_CHANNEL);
            MemSat_Timing_Profile_Start();
            // Decrypt 10 times
            i = 0;
            for (i = 0; i < 10; i++) {
                aes_decrypt(&MEMSAT_UART_Rx, API_KEY, AES_KEY);
            }
            MemSat_Timing_Profile_Report(UART_CHANNEL);
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_COMMS
            onReceiveCommsMessage(MEMSAT_UART_Rx, uart_size);
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_TIMING_TIMING
            // Convert UART to a number
            uint8_t uart_data[] = {
                // Turn chars into uint8s
                (uint8_t) MEMSAT_UART_Rx[0],
                (uint8_t) MEMSAT_UART_Rx[1],
                (uint8_t) MEMSAT_UART_Rx[2],
                (uint8_t) MEMSAT_UART_Rx[3],
            };
            uint32_t delay = uint8_array_to_uint32(&uart_data);
            MemSat_Timing_Profile_Start();
            Task_Schedule(&MemSat_Timing_Profile_Report_Default, 0, delay, 0);
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_FLASH_TIMING
            // Wipe the block first
            MemSat_Timing_Profile_Start();

            AT25DF641AStatus status;
            at25df641a_enableWrite(&spi_flash);
            at25df641a_getStatus(&spi_flash, &status);
            at25df641a_disableProtect(&spi_flash, 0x00);
            at25df641a_enableWrite(&spi_flash);
            at25df641a_getStatus(&spi_flash, &status);

            FLASH_Bulk_Erase(&at25df641a, &spi_flash, 0x00);
            MemSat_Timing_Profile_Report(UART_CHANNEL);
            // Write 4000 times
            int i = 0;
            MemSat_Timing_Profile_Start();
            for (i = 0; i < 4000; i++) {
                FLASH_Write(&at25df641a, &spi_flash, i, 'h', 1);
            }
            MemSat_Timing_Profile_Report(UART_CHANNEL);
            // Now read 4000 times
            i = 0;
            uint8_t reply[1] = {0};
            MemSat_Timing_Profile_Start();
            for (i = 0; i < 4000; i++) {
                FLASH_Read(&at25df641a, &spi_flash, i, &reply, 1);
            }
            MemSat_Timing_Profile_Report(UART_CHANNEL);
#endif
            uart_size = 0;
            uart_desired_length = 0;
        }
    }
#elif FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_PAYLOAD
    if ((c == 254 || c == 0) && uart_chars_received++ == 0) {
        // Ignore.
    } else if (payload_action == PAYLOAD_ACTION_INITALIZE) {
        if (c == 0x05) {
            payload_action = PAYLOAD_ACTION_COUNT;
            UART_WriteByte(UART_CHANNEL, PAYLOAD_ACTION_COUNT);
        } else {
            // Something is wrong!
            // We need to exit by sending an invalid character
            UART_WriteByte(UART_CHANNEL, 0x06);
        }
    } else if (payload_action == PAYLOAD_ACTION_COUNT) {
        payload_samples = c; // Breaks for 255+ samples
        if (payload_samples > 0) {
            payload_action = PAYLOAD_ACTION_READ;
            UART_WriteByte(UART_CHANNEL, PAYLOAD_ACTION_READ);
        } else {
            // Let's send a 'Finished' byte to the test
            UART_WriteByte(UART_CHANNEL, 0x03);
        }
    } else if (payload_action == PAYLOAD_ACTION_READ) {
        payload_rx[payload_rx_length++] = c;
        if (payload_rx_length == PAYLOAD_DATA_SIZE) {
            payload_packet data = Payload_Process(payload_rx);
            // In the future we can log this data.
            // Send parts of data back to verify splice.
            UART_WriteByte(UART_CHANNEL, 4);
            UART_WriteByte(UART_CHANNEL, data.chipIdentifier & 0x8f);
            UART_WriteByte(UART_CHANNEL, data.memoryAddress & 0x8f);

            payload_rx_length = 0;
            if (--payload_samples <= 0) {
                // Need more samples
                payload_action = PAYLOAD_ACTION_COUNT;
                UART_WriteByte(UART_CHANNEL, PAYLOAD_ACTION_COUNT);
            } else {
                // Read more samples
                UART_WriteByte(UART_CHANNEL, PAYLOAD_ACTION_READ);
            }
        }
    }
#endif
}
