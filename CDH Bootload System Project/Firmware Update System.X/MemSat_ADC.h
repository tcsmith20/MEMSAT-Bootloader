/* @fil MemSat_ADC.h
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Function prototypes for the analog-digital converter module
 * on the PIC24FJ256GA110. Some of this code has been auto-generated.
 */

#ifndef MEMSAT_ADC_H
#define	MEMSAT_ADC_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>
#include "MemSat_SYSTEM.h"
typedef enum 
{
    ADC_CHANNEL_AN8 =  0x8,
    ADC_CHANNEL_AN9 =  0x9,
    ADC_CHANNEL_AN10 =  0xA,
    ADC_CHANNEL_VBG_2 =  0x10,
    ADC_CHANNEL_VBG =  0x11,
    ADC_MAX_CHANNEL_COUNT = 5
} ADC_CHANNEL;

/* @fnc ADC_Init()
 * @dsc Sets the necessary configuration registers to use the ADC module in the
 * context of MemSat. By default, three channels of ADC are readied, and conversions
 * must be called for each channel separately.
 */
void ADC_Init (void);

/* @fnc ADC_Start()
 * @dsc Begins a sampling period by setting the SAMP bit high.
 */

void ADC_Start(void);

/* @fnc ADC_Stop()
 * @dsc Ends a sampling period by setting the SAMP bit low.
 */
void ADC_Stop(void);

/* @fnc ADC_ConversionResultGet()
 * @dsc Gets the value stored in the ADC result buffer.
 * @ret 10-bit ADC conversion value stored in a 16 bit number.
 */
uint16_t ADC_ConversionResultGet(void);


/* @fnc ADC_IsConversionComplete()
 * @dsc Checks whether or not the current pending conversion has finished.
 * @ret Boolean -- true: conversion is finished; false: conversion has not finished.
 */
bool ADC_IsConversionComplete( void );

/* @fnc ADC_ChannelSelect()
 * @dsc Sets the selected channel to the one passed in the argument.
 * @par ADC_CHANNEL type that determines the desired ADC channel.
 */
void ADC_ChannelSelect( ADC_CHANNEL channel );

void ADC_Tasks(void);

/* @fnc ADC_Get()
 * @dsc Performs all necessary functions needed to sample and convert an analog value.
 * @par ADC_CHANNEL type that determines the desired ADC channel.
 * @ret 10-bit conversion result stored in a 16-bit number.
 */
uint16_t ADC_Get(ADC_CHANNEL);

#endif	/* MEMSAT_ADC_H */

