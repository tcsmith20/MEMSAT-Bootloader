#include "xc.h"
#include "MemSat_RTC.h"

const uint8_t RTC_ADDR = 0x68;    //RTC address
const uint8_t CLK_REG = 0x00;     //Time Reg (0x00 - 0x07)
const uint8_t ALM_REG = 0x0a;     //Alarm Reg (0x0a - 0x0e)
const uint8_t HAL_REG = 0x0c;     //Halt Reg
const uint8_t OF_REG = 0x0f;
const uint8_t ST_REG = 0x01;
const int time_Byte = 8;
const int alarm_Byte = 5;
const int one_Byte = 1;

//---------------------------------------------------------
//  The conv_RTCR_to_dateTime and conv_dateTime_to_RTCR
//  function is use for converting dateTime formate from
//  or to RTC register (BCD)formate.
void conv_RTCR_to_dateTime(dateTime_t* time, char* data)
{
    unsigned char buffer;
    unsigned char cont_buffer;
    
    buffer = data[1] & 0x0F;
    cont_buffer = data[1] & 0x70;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->SECOND = buffer;
    
    buffer = data[2] & 0x0F;
    cont_buffer = data[2] & 0x70;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->MINUTE = buffer;
    
    buffer = data[3] & 0x0F;
    cont_buffer = data[3] & 0x30;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->HOUR = buffer;
    
    buffer = data[5] & 0x0F;
    cont_buffer = data[5] & 0x30;
    cont_buffer >>= 4;
    while(cont_buffer != 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->DAY = buffer;
    
    buffer = data[6] & 0x0F;
    cont_buffer = data[6] & 0x10;
    cont_buffer >>= 4;
    while(cont_buffer != 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->MONTH = buffer;
    
    buffer = data[7] & 0x0F;
    cont_buffer = data[7] & 0xF0;
    cont_buffer >>= 4;
    while(cont_buffer != 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    time->YEAR = buffer;
}

void conv_dateTime_to_RTCR(dateTime_t* time, char* data)
{
    unsigned char buffer;
    unsigned char cont_buffer;
    
    data[0] = 0;
    
    buffer = time->SECOND & 0x7F;    //Force stop bit is 0
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[1] = cont_buffer;
    data[1] <<= 4;
    data[1] += buffer;
    
    buffer = time->MINUTE & 0x7F;    //Bit 7 should be 0
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[2] = cont_buffer;
    data[2] <<= 4;
    data[2] += buffer;
    
    buffer = time->HOUR & 0x3F;    //Force CB and CEB be 0
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[3] = cont_buffer;
    data[3] <<= 4;
    data[3] += buffer;

    data[4] = 0;
    
    buffer = time->DAY & 0x3F;    //Bit 7 and 6 should be 0
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[5] = cont_buffer;
    data[5] <<= 4;
    data[5] += buffer;
    
    buffer = time->MONTH & 0x1F;    //Bit 7, 6 and 5 should be 0
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[6] = cont_buffer;
    data[6] <<= 4;
    data[6] += buffer;
    
    buffer = time->YEAR & 0xFF;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    data[7] = cont_buffer;
    data[7] <<= 4;
    data[7] += buffer;
}

//  The conv_RTCR_to_alarmTime and conv_alarmTime_to_RTCR
//  function is use for converting alarmTime formate from
//  or to RTC register (BCD)formate.
//  bits: 7    6    5    4    3    2    1    0
//  RPT :XXX  XXX  AFE  RP5  RP4  RP3  RP2  RP1
void conv_RTCR_to_alarmTime(alarmTime_t* alarm, char* data)
{
    unsigned char buffer;
    unsigned char cont_buffer;
    
    buffer = data[0] & 0x0F;
    cont_buffer = data[0] & 0x10;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    alarm->MONTH = buffer;
    
    buffer = data[1] & 0x0F;
    cont_buffer = data[1] & 0x30;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    alarm->DAY = buffer;
    
    buffer = data[2] & 0x0F;
    cont_buffer = data[2] & 0x30;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    alarm->HOUR = buffer;
    
    buffer = data[3] & 0x0F;
    cont_buffer = data[3] & 0x70;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    alarm->MINUTE = buffer;
    
    buffer = data[4] & 0x0F;
    cont_buffer = data[4] & 0x70;
    cont_buffer >>= 4;
    while(cont_buffer > 0)
    {
        buffer += 10;
        cont_buffer--;
    }
    alarm->SECOND = buffer;
    
    buffer = data[0] & 0x80;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
    alarm->RPT <<= 1;
    buffer = data[1] & 0x40;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
    alarm->RPT <<= 1;
    buffer = data[1] & 0x80;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
    alarm->RPT <<= 1;
    buffer = data[2] & 0x80;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
    alarm->RPT <<= 1;
    buffer = data[3] & 0x80;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
    alarm->RPT <<= 1;
    buffer = data[4] & 0x80;
    if(buffer > 0)
    {
        alarm->RPT += 1;
    }
}

void conv_alarmTime_to_RTCR(alarmTime_t* alarm, char* data)
{
    unsigned char buffer;
    unsigned char cont_buffer;

    buffer = alarm->MONTH & 0x1F;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    if(alarm->RPT && 0x20)
    {
        cont_buffer += 0x80;
    }
    data[0] = cont_buffer;
    data[0] <<= 4;
    data[0] += buffer;
    
    buffer = alarm->DAY & 0x3F;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    if(alarm->RPT && 0x10)
    {
        cont_buffer += 0x40;
    }
    if(alarm->RPT && 0x08)
    {
        cont_buffer += 0x80;
    }
    data[1] = cont_buffer;
    data[1] <<= 4;
    data[1] += buffer;
    
    buffer = alarm->HOUR & 0x3F;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    if(alarm->RPT && 0x04)
    {
        cont_buffer += 0x80;
    }
    data[2] = cont_buffer;
    data[2] <<= 4;
    data[2] += buffer;
    
    buffer = alarm->MINUTE & 0x7F;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    if(alarm->RPT && 0x02)
    {
        cont_buffer += 0x80;
    }
    data[3] = cont_buffer;
    data[3] <<= 4;
    data[3] += buffer;
    
    buffer = alarm->SECOND & 0x7F;
    while(buffer > 9)
    {
        cont_buffer++;
        buffer -= 10;
    }
    if(alarm->RPT && 0x01)
    {
        cont_buffer += 0x80;
    }
    data[4] = cont_buffer;
    data[4] <<= 4;
    data[4] += buffer;
}

//---------------------------------------------------------

//Time is 8 byte array; mSecond only can set to 00.
//  [0]   -  [1]   -   [2]  - [3]  - [4] - [5]  -  [6]  -  [7]
//mSecond - second - minute - hour - day - date - month - year
void RTC_Set(dateTime_t* time)
{
    char data[8];
    conv_dateTime_to_RTCR(time, data);
    I2C1_M_Write(RTC_ADDR, CLK_REG, time_Byte, data);
}

// dateTime_t will be 0s if read function fail.
void RTC_Get(dateTime_t* time)
{
    char data[8];
    
    int i = I2C1_M_Read(RTC_ADDR, CLK_REG, time_Byte, data);
    
    if(i == 0)
    {
        conv_RTCR_to_dateTime(time, data);
    }
    else
    {
        time->SECOND = 0;
        time->MINUTE = 0;
        time->HOUR = 0;
        time->DAY = 0;
        time->MONTH = 0;
        time->YEAR = 0;
    }
}

void RTC_Set_Alram(alarmTime_t* alarm)
{
    char data[5];
    conv_alarmTime_to_RTCR(alarm, data);
    I2C1_M_Write(RTC_ADDR, ALM_REG, alarm_Byte, data);
}

void RTC_Get_Alram(alarmTime_t* alarm)
{
    char data[5];
    
    int i = I2C1_M_Read(RTC_ADDR, ALM_REG, alarm_Byte, data);
    
    if(i == 0)
    {
        conv_RTCR_to_alarmTime(alarm, data);
    }
    else
    {
        alarm->SECOND = 0;
        alarm->MINUTE = 0;
        alarm->HOUR = 0;
        alarm->DAY = 0;
        alarm->MONTH = 0;
        alarm->RPT = 0;
    }
}

//---------------------------------------------------------

//Function call after launch.
void RTC_Init(void)
{
    //RTC time set to: 00ms, 00s, 00m, 00h, Mon, 1st, Jan, 2018.
    char time[8] = {0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0xA8};
    //RTC alarm set to: Jan 1st, 0:45:00, repeat every hour.
    char alarm[5] = {0xA0, 0xC0, 0x80, 0x45, 0x00};
    
    I2C_SWini();
    I2C_HWini();
    I2C_ModuleStart();
    
    while(I2C1_M_Poll(RTC_ADDR) != 0)  // poll the I2C bus
    {
        __delay32(30);
    }
    
    //set alarm and start the clock
    I2C1_M_Write(RTC_ADDR, CLK_REG, time_Byte, time);
    I2C1_M_Write(RTC_ADDR, ALM_REG, alarm_Byte, alarm);
}

//This function can call when first time applying 
//power to RTC, and when Oscillator stop running.
//RTC time need to be updated after this function.
void start_Oscillator(void)
{
    char high = 0x80;
    char low = 0x00;
    char second;
    
    I2C1_M_Write(RTC_ADDR, OF_REG, one_Byte, &low);   //Set Oscillator Flag low
    
    int i = I2C1_M_Read(RTC_ADDR, ST_REG, one_Byte, &second);   //Copying time
    second &= 0x7F;
    
    I2C1_M_Write(RTC_ADDR, ST_REG, one_Byte, &high);  //Restart oscillator by setting ST bit high then low
    
    if(i == 0)
    {
        I2C1_M_Write(RTC_ADDR, ST_REG, one_Byte, &second);
    }
    else
        I2C1_M_Write(RTC_ADDR, ST_REG, one_Byte, &low);
}

//call this after brownout occur to get the 
//brownout time and restart updating the clock reg.
//buffer should be 8 bytes to read the full time.
//return 0 for Success or 0x8001 for error, 0x8002 for time is not halted.
int Brownout_Time(dateTime_t* time)
{
    char halt;
    int size = 1;
    char data[8];
    
    int i = I2C1_M_Read(RTC_ADDR, HAL_REG, sizeof(halt), &halt);
    if(i != 0)
        return 0x8001;
    
    if(halt && 0x40)
        return 0x8002;
    
    if((halt && 0x40) == 0x40)
    {
        i = I2C1_M_Read(RTC_ADDR, CLK_REG, sizeof(data), data);
        if(i != 0)
            return 0x8001;
    }
        
    conv_RTCR_to_dateTime(time, data);
        
    //set halt bit to 0 to allow updating the clk reg.
    halt &= 0xbf;
    I2C1_M_Write(RTC_ADDR, HAL_REG, size, &halt);
    return 0;
}

//This function read reg 00h - 0Fh in the RTC
//Returns:
//0x0000 Success
//0x8001 Error
int RTC_Regs_Read(char* buffer)
{
    int status;
    uint8_t bytes = 16;

    status = I2C1_M_Read(RTC_ADDR, CLK_REG, bytes, buffer);
    
    return status;
}