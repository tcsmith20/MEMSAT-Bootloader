/* 
 * File:   main.h
 * Author: firem
 *
 * Created on April 5, 2017, 4:15 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "system.h"
#include "MemSat_SYSTEM.h"
#include "MemSat_RTC.h"

void Power_Health();
int main();
void togglePINF1();
void Print_Time();
//-----------------------------------
void I2C_Module_Test();
void Power_Ping_Test();
void Power_module_test();
void MemSat_Launch();

#endif	/* MAIN_H */

