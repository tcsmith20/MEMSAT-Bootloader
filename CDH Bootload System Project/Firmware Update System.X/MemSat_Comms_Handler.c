#include "MemSat_Comms_Handler.h"

#define MSG_TYPE_OTA_UPDATE 1

void MEMSAT_Comms_On_Receive(char msg[], char msg_length) {
    // Check the first byte to get the type
    switch (msg[0]) {
        case MSG_TYPE_OTA_UPDATE: {
            // Get page id
            int pageId = msg[1];
            pageId = pageId << 8; // Make this first of two bytes
            pageId += msg[2];
            // Get total page size
            int pageTotal = msg[3];
            pageTotal = pageTotal << 8; // Make this first of two bytes
            pageTotal += msg[4];
            // Get the raw data
            char dataBlob[msg_length - 5]; // Get all but first five
//             Mem copy
            char dataLength = msg_length - 5; // Allocate final size first
            uint8_t i = 0;
            for (i = 0; i < dataLength; i++) {
                dataBlob[i] = msg[i + 5];
            }
            MEMSAT_Comms_OTA_Handle_OTA_Page(dataBlob, pageId, pageTotal);
            break;
        }
    }
    
}

void MEMSAT_Comms_OTA_Handle_OTA_Page(char dataBlob[], int pageId, int pageTotal) {
    // Write to memory
    UART_WriteByte(UART_CHANNEL, '5');
}