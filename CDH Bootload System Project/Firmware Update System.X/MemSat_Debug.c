#include "MemSat_Debug.h"
#include "main.h"

dateTime_t testDate, testDate2;

void debug_options() {
    
#if FEATURE_UART_RESPONSE == UART_RESPONSE_TIMING_TIMING
void Report_Timing();
#endif
    
#if FEATURE_BLINKY == TRUE
    Task_Schedule(togglePINF1, 0, 0, 500);
#endif

#if MODE == MODE_DEBUG
    //Task_Schedule(I2C_Test,0,0,2000);
    //Task_Schedule(RTC_Test,0,0,500);
    //Task_Schedule(payload_repeat,0,250,500);

#endif

    // TODO Check whether we already initialized from a Flash flag
#if DEBUG_UART_PRINTF == TRUE
    MEMSAT_DEBUG_UART_Init();
    //UART_Printf(UART_CHANNEL, "Good afternoon everyone!\n\r");
    //UART_Printf(UART_CHANNEL, "Deploy the antenna after some time\n\r");
    //Task_Schedule(&Print_Time, 0, 1000, 1000);
#endif

}

void MEMSAT_DEBUG_UART_Init(){
    UART_Init(UART_CHANNEL);
}

void payload_repeat() {
    static uint8_t data_in[8];
    static uint8_t data_out[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    SPI2_ExchangeBuffer(data_out, 8, data_in);
}

void togglePINF1() {
    PORTFbits.RF1 ^= 1;
}

void SPI_Test() {
    uint8_t testData[] = {0x0, 0x10, 0x20, 0x30, 0x40, 0x50};
    uint8_t receiveData[sizeof (testData)];
    SPI2_ExchangeBuffer(testData, sizeof (testData), receiveData);
}

void RTC_Test() {
    testDate2.YEAR = 0x86;
    testDate2.MONTH = 0x11;
    testDate2.DAY = 0x29;
    testDate2.HOUR = 0x30;
    testDate2.SECOND = 0x26;
    testDate2.MINUTE = 0x11;

    //RTC_Set(&testDate2);
    //RTC_Get(&testDate);

    SPI2_Exchange8bit(testDate2.YEAR);
    SPI2_Exchange8bit(testDate2.MONTH);
    SPI2_Exchange8bit(testDate2.DAY);
    SPI2_Exchange8bit(testDate2.HOUR);
    SPI2_Exchange8bit(testDate2.MINUTE);
    SPI2_Exchange8bit(testDate2.SECOND);

    SPI2_Exchange8bit(testDate.YEAR);
    SPI2_Exchange8bit(testDate.MONTH);
    SPI2_Exchange8bit(testDate.DAY);
    SPI2_Exchange8bit(testDate.HOUR);
    SPI2_Exchange8bit(testDate.MINUTE);
    SPI2_Exchange8bit(testDate.SECOND);

    UART_Printf(UART_CHANNEL, "%d:%d:%d ", testDate.HOUR, testDate.MINUTE, testDate.SECOND);

    RCFGCALbits.RTCPTR = 0b11;

}