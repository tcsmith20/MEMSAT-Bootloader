/* 
 * File:   MemSat_Timing_Profile.h
 * Author: firem
 *
 * Created on March 3, 2017, 1:25 PM
 */

#include "system.h"


#if FEATURE_TIMING_PROFILES == TRUE
#ifndef MEMSAT_TIMING_PROFILE_H
#define	MEMSAT_TIMING_PROFILE_H

void MemSat_Timing_Profile_Start();
void MemSat_Timing_Profile_Report_Default();
uint32_t MemSat_Timing_Profile_Report(char);
char * uint32_to_char_array(uint32_t bytes);
uint32_t uint8_array_to_uint32(uint8_t * bytes);

#endif	/* MEMSAT_TIMING_PROFILE_H */
#endif
