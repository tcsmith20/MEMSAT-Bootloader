/* @fil MemSat_ADC.c
 * @dat 12/12/2016
 * @dev Michael McCaffrey
 * @dsc Function implementations for the analog-digital converter module
 * on the PIC24FJ256GA110. Some of this code has been auto-generated.
 */
#include <xc.h>
#include "MemSat_ADC.h"

typedef struct
{
	uint8_t intSample;
} ADC_OBJECT;

static ADC_OBJECT adc1_obj;

void ADC_Init (void)
{
    // Pin AN8 to Analog
   AD1PCFG = 0;
    // ASAM disabled; ADSIDL disabled; DONE disabled; FORM Absolute decimal result, unsigned, right-justified; SAMP disabled; SSRC Clearing sample bit ends sampling and starts conversion; ADON enabled; 
   AD1CON1 = 0x8000;
    // CSCNA disabled; VCFG0 AVDD; VCFG1 AVSS; ALTS disabled; BUFM disabled; SMPI 1; OFFCAL disabled; 
   AD1CON2 = 0x0000;
    // SAMC 0; ADRC FOSC/2; ADCS 0; 
   AD1CON3 = 0x0000;
    // CH0SA AN0; CH0SB AN0; CH0NB AVSS; CH0NA AVSS; 
   AD1CHS = 0x0000;
    // CSSL17 disabled; CSSL16 disabled; 
   AD1CSSH = 0x0000;
    // CSSL9 disabled; CSSL8 disabled; CSSL7 disabled; CSSL6 disabled; CSSL5 disabled; CSSL4 disabled; CSSL3 disabled; CSSL2 disabled; CSSL15 disabled; CSSL1 disabled; CSSL14 disabled; CSSL0 disabled; CSSL13 disabled; CSSL12 disabled; CSSL11 disabled; CSSL10 disabled; 
   AD1CSSL = 0x0000;
   adc1_obj.intSample = AD1CON2bits.SMPI;
   
}

void ADC_Start(void)
{
   AD1CON1bits.SAMP = 1;
}

void ADC_Stop(void)
{
   AD1CON1bits.SAMP = 0;
}

uint16_t ADC_ConversionResultGet(void)
{
    return ADC1BUF0;
}

bool ADC_IsConversionComplete( void )
{
    return AD1CON1bits.DONE; //Wait for conversion to complete   
}

void ADC_ChannelSelect( ADC_CHANNEL channel )
{
    AD1CHS = channel;
}

uint16_t ADC_Get(ADC_CHANNEL channel){
    uint16_t conversion;
    ADC_ChannelSelect(channel);
    ADC_Start();
    //Provide Delay
    DelayMs(32);
    ADC_Stop();
    while(!ADC_IsConversionComplete())
    {
        ADC_Tasks();   
    }
    conversion = ADC_ConversionResultGet();

    return conversion;
}

void ADC_Tasks ( void )
{
    // clear the ADC interrupt flag
    IFS0bits.AD1IF = false;
}