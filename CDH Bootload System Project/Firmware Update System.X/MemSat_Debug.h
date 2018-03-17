/* 
 * File:   MemSat_Debug.h
 * Author: Mike
 *
 * Created on May 25, 2017, 4:17 PM
 */

#ifndef MEMSAT_DEBUG_H
#define	MEMSAT_DEBUG_H

#ifdef	__cplusplus
extern "C" {
#endif

    void debug_options();
    void togglePINA0();
    void togglePINF1();
    void MEMSAT_DEBUG_UART_Init();
    void payload_repeat();
    void I2C_Test();
    void SPI_Test();
    void RTC_Test();


#ifdef	__cplusplus
}
#endif

#endif	/* MEMSAT_DEBUG_H */

