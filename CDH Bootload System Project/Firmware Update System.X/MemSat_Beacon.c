#include "MemSat_SYSTEM.h"
#include "system.h"
#include "MemSat_Beacon.h"
#include "timing.h"
#include "task.h"

void Beacon_Test();

char *init_cmd[] = {
    "CK2DAD\r",
    "DROWANU\r",
    "VNOTAPP\r",
    "F8E72D\r",
    "P01\r",
    "M1200\r"
};

unsigned char state = 1;
void (*callback)();

void Beacon_Init(void* readycallback) {
    callback = readycallback;
    UART_Printf(UART_CHANNEL, init_cmd[0]); // Write first string.
    //Task_Schedule(MemSat_Beacon_Check_Status, 0, 100, 0); // Schedule once
#if FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_BEACON_FLASH
    Task_Schedule(&MemSat_Beacon_Print_Flash, 0, 100, 0);
#endif
}

void Beacon_Setup() {
    if (!Beacon_Initialized()) {
        if (MEMSAT_UART_Beacon_Status() != 'K') {
            return; // Invalid state
        }
        //PORTA ^= 0xff;

        UART_Printf(UART_CHANNEL_BEACON, init_cmd[state]);
        state++;

        if (Beacon_Initialized()) {
            (* callback)();
        }
    }
}

void Beacon_Write(char * buffer, uint8_t buffer_length) {
    // Write an 'S'
    UART_WriteByte(UART_CHANNEL_BEACON, 'S');
    // Write ASCII
    UART_Printf(UART_CHANNEL_BEACON, buffer);
    // Write a carriage return
    UART_WriteByte(UART_CHANNEL_BEACON, '\r');
}

int Beacon_Initialized() {
    if (state >= 5) {
        // We are initialized
        return TRUE;
    }
    return FALSE;
}

#if FEATURE_UART_RESPONSE == UART_RESPONSE_MOCK_BEACON_FLASH
uint32_t addr = 0;

void MemSat_Beacon_Print_Flash() {
    PORTA ^= 0xff;
    static uint8_t i = 0;
    uint8_t reply0[] = "Welcome to Rowan!";
    uint8_t reply1[] = "This is the Rowan MEMSAT satellite project!";
    uint8_t reply2[] = "Ask any questions you want to our wonderful student volunteers!";
    if (Beacon_Initialized() == TRUE) {
        // Pull from flash
        switch (i) {
            case 0: Beacon_Write(&reply0, sizeof (reply0));
                break;
            case 1: Beacon_Write(&reply1, sizeof (reply1));
                break;
            case 2: Beacon_Write(&reply2, sizeof (reply2));
                break;
        }
        i = i > 1 ? 0 : i + 1;
    }

    //        FLASH_Read(&at25df641a, &spi_flash, addr, &reply, sizeof(reply));
    //MemSat_Beacon_Write(&reply, sizeof(reply));
    addr += 0x10;
    addr = addr % 65000;

    Task_Schedule(&MemSat_Beacon_Print_Flash, 0, 10000, 0);
}
#endif