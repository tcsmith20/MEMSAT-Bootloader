#include <stdint.h>
#include "MemSat_Payload.h"

/* A 6xN matrix of data */
uint8_t payload_buffer[PAYLOAD_BUFFER_MAX][PAYLOAD_DATA_SIZE];
uint8_t payload_buffer_length = 0;

/* Bit array of alive (1) or dead (0) chips */
uint32_t chip_status;

payload_packet Payload_Process(uint8_t data[]) {
    // Should be a 6-byte packet.
    payload_packet packet;
    packet.chipIdentifier = (data[0] & 0b11111000) >> 3;
    packet.memoryAddress = data[0] & 0b00000111;
    packet.memoryAddress = (packet.memoryAddress << 8) | data[1];
    packet.memoryAddress = (packet.memoryAddress << 5);
    packet.memoryAddress = packet.memoryAddress | ((data[2] & 0b11111000) >> 3);
    packet.latchOrFlipped = (data[2] & 0b00000100) >> 2;
    packet.temperature = data[2] & 0b00000011;
    packet.temperature = (packet.temperature << 8) | data[3];
    packet.temperature = (packet.temperature << 2) | ((data[4] & 0b11000000) >> 6);
    packet.radiation = data[4] & 0b00111111;
    packet.radiation = (packet.radiation << 8) | (data[5] & 0b11111100);
    packet.radiation = packet.radiation >> 2; // Adjust for end of packet
    return packet;
}

void Payload_Log() {
    int i;
    for (i = 0; i < payload_buffer_length; i++) {
        // Get realtime clock data
        // Generate a larger packet
        // Throw into Flash
    }
    payload_buffer_length = 0;
}

void Payload_Log_Chip_Fail(uint8_t chip) {
    static uint8_t payload_log_cursor = 0;
    uint16_t payload_log_addr = FLASH_PAYLOAD_LOGS + payload_log_cursor;
    
    // Get realtime clock data
    volatile uint32_t time = TimeNow();

    // Create log data array
    uint8_t data[PAYLOAD_LOG_ENTRY_SIZE] = {
        chip,
        (time >> 24) & 0xff,
        (time >> 16) & 0xff,
        (time >> 8) & 0xff,
        time & 0xff
    };
    
#if MODE == MODE_DEBUG
    UART_Printf(UART_CHANNEL, "Logging chip death");
#endif
    
    // Log data in flash
    Payload_Flash_Write(&at25df641a, &spi_flash, payload_log_addr, data, PAYLOAD_LOG_ENTRY_SIZE);
    
    // Increment or reset cursor
    payload_log_cursor = payload_log_addr > (FLASH_PAYLOAD_LOGS_END - PAYLOAD_LOG_ENTRY_SIZE) ? 
        payload_log_cursor + PAYLOAD_LOG_ENTRY_SIZE : 0;
    
}

void Payload_Get_Status() {
    // check for newly dead chips
    // if new dead chip, log death
    if (Payload_Query(PAYLOAD_ACTION_STATUS)) {
        volatile uint32_t new_chip_status = SPI2_Exchange8bit(chip_status);
        if (new_chip_status != chip_status) {
            volatile uint32_t dead_chip = new_chip_status ^ chip_status;
            volatile uint8_t dead_chip_num = 0;
            while (dead_chip) {
                dead_chip_num++;
                dead_chip >>= 1;
            }
            Payload_Log_Chip_Fail(dead_chip_num);
        }
    }
}

uint32_t Payload_Chip_Status() {
    return chip_status;
}

uint8_t Payload_Buffer_Length() {
    return payload_buffer_length;
}

uint8_t * Payload_Buffer_Get(uint8_t index) {
    return (payload_buffer[index]);
}

uint32_t Payload_Data_Available() {
    volatile uint8_t i = 3;
    uint8_t available[4] = {0, 1, 2, 3};
    uint32_t total = 0;
    if (Payload_Query(PAYLOAD_ACTION_COUNT)) {
        SPI2_ExchangeBuffer(available, sizeof (available), available);

        while (i--) {
            total += available[i];
            total <<= 8;
        }

        return total;
    } else return 0xffffffff;
}

void Payload_Read() {
    // Use SPI
    // Send 0b10
    // Read 6 bytes into buffer and move to next buffer word while data available
    while (Payload_Data_Available()) {
        SPI2_ExchangeBuffer(payload_buffer[payload_buffer_length], 6, payload_buffer[payload_buffer_length]);
        payload_buffer_length++;
        if (payload_buffer_length >= PAYLOAD_BUFFER_MAX){ 
            //Payload_Log();
            // RESET BUFFER CURSOR
        }
    }

    //    payload_buffer[payload_buffer_length][0] = 0;
    //    payload_buffer[payload_buffer_length][1] = 0;
    //    payload_buffer[payload_buffer_length][2] = 0;
    //    payload_buffer[payload_buffer_length][3] = 0;
    //    payload_buffer[payload_buffer_length][4] = 0;
    //    payload_buffer[payload_buffer_length++][5] = 0;
}

void Payload_Send_Init() {
    if (Payload_Query(PAYLOAD_ACTION_INITALIZE)) {
        if (Task_IsScheduled(Payload_Send_Init))
            Task_Remove(Payload_Send_Init, 0);
        Payload_Get_Status();
    }
    // Use SPI
    // Send 0b101 (5)
    // Read 6 bytes and process into a status byte
}

uint8_t Payload_Query(uint8_t message) {
    return SPI2_Exchange8bit(message);
}

uint8_t Payload_Ready() {
    return Payload_Query(PAYLOAD_ACTION_ROLLCALL);
}