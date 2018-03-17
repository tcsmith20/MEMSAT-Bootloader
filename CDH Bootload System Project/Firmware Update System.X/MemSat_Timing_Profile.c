// Only provide code if timing profiles are enabled. Otherwise it's unneeded.
#if FEATURE_TIMING_PROFILES == TRUE

#include "system.h"
#include "MemSat_Timing_Profile.h"

uint32_t time_start;
char bytes8[4];
/*
 * Records the current time to begin the test.
 */
void MemSat_Timing_Profile_Start() {
    time_start = TimeNow();
}

void MemSat_Timing_Profile_Report_Default() {
    MemSat_Timing_Profile_Report(UART_CHANNEL);
}

/*
 * Marks the end of the test and sends the result over UART.
 * 
 * @param uart_channel The microcontroller UART channel to send data across.
 * @return The test length in milliseconds.
 */
uint32_t MemSat_Timing_Profile_Report(char uart_channel) {
    uint32_t result = TimeNow() - time_start;
    // Now try to transfer 4 bytes individually.
    char * byteArray = uint32_to_char_array(result);
    char b0 = byteArray[0];
    char b1 = byteArray[1];
    char b2 = byteArray[2];
    char b3 = byteArray[3];
    UART_WriteByte(UART_CHANNEL, b0);
    UART_WriteByte(UART_CHANNEL, b1);
    UART_WriteByte(UART_CHANNEL, b2);
    UART_WriteByte(UART_CHANNEL, b3);
    return result;
}

char * uint32_to_char_array(uint32_t bytes32) {
    bytes8[0] = (bytes32 >> 24) & 255;
    bytes8[1] = (bytes32 >> 16) & 255;
    bytes8[2] = (bytes32 >> 8) & 255;
    bytes8[3] = bytes32 & 255;
    return bytes8;
}

uint32_t uint8_array_to_uint32(uint8_t * bytes) {
    uint32_t out = 0;
    volatile uint8_t i = 0;
    
    while(i < 4){
        out += bytes[i++];
        out <<= 8;
    }
    
    return out;
}

#endif

