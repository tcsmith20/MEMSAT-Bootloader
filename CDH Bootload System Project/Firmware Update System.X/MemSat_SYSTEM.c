#include "MemSat_SYSTEM.h"

void MemSat_Antenna_Deploy() {
    LATEbits.LATE5 = 1;
#if DEBUG_UART_PRINTF == TRUE
    PORTA |= 0xff;
    UART_Printf(UART_CHANNEL, "Antenna is deployed! Blam!\r\n");
    UART_Printf(UART_CHANNEL, "Start comms after some time\r\n");
#endif
    Task_Schedule(&MemSat_Antenna_Init_Comms, 0, (unsigned long) TIME_ELAPSED_START_COMMS, 0);
}

void MemSat_Antenna_Init_Comms() {
    // Do a full system startup
#if DEBUG_UART_PRINTF == TRUE
    UART_Printf(UART_CHANNEL, "Starting to communicate\r\n");
#endif
    Task_Schedule(&MemSat_System_Init, 0, 1, 0);
}

void MemSat_System_Init() {

#if DEBUG_UART_PRINTF == TRUE
    UART_Printf(UART_CHANNEL, "Init subsystems\n\r");
#else
    
#endif
        
    TRISA = 0xFF00;
    _TRISF1 = 0;
    TRISD = 0;
    _TRISD9 = 1;
    _PMPEN = 0;
    _PTBEEN = 0;
    
    // PIC init
    MemSat_Watchdog_Disable();
    
    // Embedded library init
    Timing_Init();
    Task_Init();
    
    // Peripheral init
    ADC_Init();
    SPI2_Initialize();
    MEMSAT_UART_Init();
    //RTC_Init(current_Date_Time());
    FLASH_Init(&at25df641a, &spi_flash); // This inits SPI1
    
    // Subsystems init
    Power_Init();
    Payload_Send_Init();
    //Beacon_Init(MemSat_On_Beacon_Ready);
    
    // Device init
    
    // Comms init
    // Payload init
    // Schedule payload and other tasks
}

void MemSat_On_Beacon_Ready() {
    UART_Write(UART_CHANNEL, "We are ready to transmit", 24);
}

void MemSat_Watchdog_Disable() {
    #if defined(__XC16__)
//    _CONFIG1 &= FWDTEN_OFF; // Force off Watchdog timer
	RCONbits.SWDTEN = 0; /* Disable watchdog timer. */
    #else
	WDTCTL = WDTPW | WDTHOLD; /* Disable watchdog timer. */
    #endif
}
