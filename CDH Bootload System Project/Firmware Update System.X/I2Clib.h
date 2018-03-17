/* 
 * File:   I2Clib.h
 * Author: David McCurley
 *
 * Created on September 12, 2013, 6:48 PM
 */

#ifndef I2CLIB_H
#define	I2CLIB_H

#include <stdint.h>
#include <libpic30.h>

//--------------------Constants--------------------
#define I2C_M_ACK	0
#define I2C_M_NACK	1

//--------------------Structs--------------------
//--------------------Variables--------------------
//--------------------Functions--------------------
extern void I2C_HWini(void);
extern void I2C_ModuleStart(void);
extern void I2C_SWini(void);
extern int  I2C1_M_BusReset(void);
extern void I2C1_M_ClearErrors(void);
extern int  I2C1_M_Poll(uint8_t);
extern int  I2C1_M_RcvByte(uint8_t);
extern int  I2C1_M_RecoverBus(void);
extern int  I2C1_M_Read(uint8_t, uint8_t, int, char *);
extern int  I2C1_M_ReStart(void);
extern int  I2C1_M_SendByte(char);
extern int  I2C1_M_Start(void);
extern int  I2C1_M_Stop(void);
extern void I2C1_M_Write(uint8_t, uint8_t, int, char *);

//-------------------Function added by SY----------------
extern int  MemSat_Power_Read(uint8_t, uint8_t, uint8_t, int, char *);
extern int  Heater_Read(uint8_t, uint8_t, int, char *, int, char *);
extern int  I2C1_Bus_Poll();
//-------------------Macros-------------------
#define SetI2C1BusDirty	I2Cflags |=  0x0001
#define ClrI2C1BusDirty	I2Cflags &= ~0x0001
#define IsI2C1BusDirty	(I2Cflags & 0x0001)

#endif	/* I2CLIB_H */

