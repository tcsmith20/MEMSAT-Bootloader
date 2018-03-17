/* 
 * File: MENSAT_RTC
 * Author: SY
 * Comments: !!!Code haven't test on hardware yet!!! 
 *           This is use to connect the RTC M41T81S chip on the MemSat C&DH board.
 * Revision history: 1.0
 */

typedef struct {
    unsigned char YEAR;
    unsigned char MONTH;
    unsigned char DAY;
    unsigned char HOUR;
    unsigned char MINUTE;
    unsigned char SECOND;
}dateTime_t;

typedef struct {
    unsigned char RPT;
    unsigned char MONTH;
    unsigned char DAY;
    unsigned char HOUR;
    unsigned char MINUTE;
    unsigned char SECOND;
}alarmTime_t;


#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h>
#include "I2Clib.h"

void conv_RTCR_to_dateTime(dateTime_t*, char*);
void conv_dateTime_to_RTCR(dateTime_t*, char*);
void conv_RTCR_to_alarmTime(alarmTime_t*, char*);
void conv_alarmTime_to_RTCR(alarmTime_t*, char*);

void RTC_Set(dateTime_t*);
void RTC_Get(dateTime_t*);
void RTC_Set_Alram(alarmTime_t*);
void RTC_Get_Alram(alarmTime_t*);

void RTC_Init(void);
void start_Oscillator(void);
int  Brownout_Time(dateTime_t*);
int  RTC_Regs_Read(char*);

#endif	/* XC_HEADER_TEMPLATE_H */

