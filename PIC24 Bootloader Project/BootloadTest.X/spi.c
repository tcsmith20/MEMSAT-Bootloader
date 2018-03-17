/*
 * spi.c
 *
 *  Created on: Oct 29, 2016
 *      Author: Nicholas DeCicco <nsd.cicco@gmail.com>
 *                               <deciccon0@students.rowan.edu>
 */
#if defined(__XC16__)
#include <xc.h>
#else
#include <msp430.h>
#endif
#include <stdint.h>
#include "spi.h"
void spi_select()
{
#if defined(__XC16__)
	_RD13 = 0;
#else
	P3OUT &= ~BIT2;
#endif
}
void spi_deselect()
{
#if defined(__XC16__)
	_RD13 = 1;
#else
	P3OUT |= BIT2;
#endif
}
// AT25DF641A
// Pin 70 (RP3/CN55/PMCS2/RD10)  -> MOSI
// Pin 71 (RP12/CN56/PMCS1/RD11) -> SCK
// Pin 72 (RP11/CN49/RD0)        <- MISO
// Pin 80 (CN19/RD13)            -> _CS_
/**
 * Initializes the USCI peripheral for SPI communications.
 */
void spi_init()
{
#if defined(__XC16__)
	SPI1STATbits.SPIEN = 0; // Disable the SPI1 peripheral
	
	// If the block diagrams of the port inputs are to be believed,
	// it is not necessary to configure a pin as an input or output when
	// the peripheral is driving the pin.
	_TRISD0  = 1; // Make MISO input
	_TRISD13 = 0; // SS to RP14 (manual control)
	_RD13    = 1; // Deselect the device
	__builtin_write_OSCCONL(OSCCON & 0xBF); // 0xBF = ~0x40
	while (OSCCONbits.IOLOCK) {} // Wait for IOLOCK to clear
	_RP12R = _RPOUT_SCK1OUT; // SCK to RP12
	_RP3R  = _RPOUT_SDO1;    // SDO to RP3
	_SDI1R = 11;             // SDI to RP11
	__builtin_write_OSCCONL(OSCCON | _OSCCON_IOLOCK_MASK);
	while (!OSCCONbits.IOLOCK) {} // Wait for IOLOCK to set
	_SPI1IF = 0; // Clear interrupt flag
	_SPI1IE = 0; // Disable interrupts for the SPI1 peripheral
	SPI1STATbits.SPISIDL = 0;
	SPI1BUF              = 0x00;
	
	SPI1CON1bits.DISSCK  = 0; // Enable internal SPI clock
	SPI1CON1bits.DISSDO  = 0; // Enable SDO1
	SPI1CON1bits.SMP     = 0; // 0 = Input data sampled at middle of data output time
	                          // 1 = Input data sampled at end of data output time
	SPI1CON1bits.CKE     = 1; // 0 = data changes on idle->active clock
	                          // 1 = data changes on active->idle clock
	SPI1CON1bits.CKP     = 0; // 0 = clock is active high
	                          // 1 = clock is active low
	SPI1CON1bits.MSTEN   = 1; // Operate in SPI master mode
	//SPI1CON1bits.SSEN  = 1; // Enable SS (CS) pin
	// These prescale options give 1 MHz clock with default device clock
	// settings
	SPI1CON1bits.SPRE    = 6; // Secondary Prescale 2:1
	SPI1CON1bits.PPRE    = 3; // Primary Prescale 1:1
	
	SPI1STATbits.SPIROV  = 0; // docs say to do this before setting SPIEN = 1
	SPI1STATbits.SPIEN   = 1; // Enable the SPI1 peripheral
	                          // (this should come last)
#else
	/* Configure ports. The MSP430F5529 has the following pin mappings:
	 * UCA0CLK  P2.7
	 * UCA0STE  P3.2
	 * UCA0SIMO P3.3
	 * UCA0SOMI P3.4
	 * Setting the PxSEL bits 'high' enables peripherals to use those pins.
	 */
	P2SEL |= BIT7;
	P3SEL |= /*BIT2 |*/ BIT3 | BIT4;
	P3DIR |= BIT2; // for doing CS ourselves
	P3OUT |= BIT2; // for doing CS ourselves
	/* Reset USCIA0 registers and disable USCIA0 by setting the UCxCTL1
	 * bit of UCSWRST.
	 */
	UCA0CTL1 |= UCSWRST;
	//phaseAndPolarity &= UCCKPL | UCCKPH;
	UCA0CTL0 |= UCMSB    | /* (Optional) Configure the USCI to send MSB first. */
	            UCMST    | /* Select master mode. */
	            UCSYNC   | /* Enable synchronous mode. */
//	            UC7BIT   | /* (Optional) Set character length to 7-bits. */
	            //UCCKPL   | /* Clock polarity: the inactive state is high. */
				UCCKPH   | /* Clock phase */
				//phaseAndPolarity |
				UCMODE_0;  /* 4-pin SPI mode with UCxSTE (chip select) active low */
	/* Set clock source to SMCLK. (If we don't set a clock source,
	 * UCSSEL will default to 0, which is "reserved"; i.e., not tied to any
	 * clock source. (Behavior undefined afaik.)
	 */
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0  = 0x02; /* divide by 2; this gives us ~0.5 MHz operation */
	UCA0BR1  = 0;
	UCA0MCTL = 0; /* No modulation */
	/* Clear UCxCTL1 to take the USCI out of the reset condition. */
	UCA0CTL1 &= ~UCSWRST;
	/* (Optional) Enable interrupts. */
//	UCA0IE |= UCRXIE;
//	UCA0IE |= UCTXIE;
#endif
}
/**
 * Sends an individual byte of data over SPI.
 */
void spi_blockingSendByte(const uint8_t data)
{
#if defined(__XC16__)
	SPI1BUF = data;
	// Wait until ready to send again
	//while (!SPI1STATbits.SRMPT) {}
	// ^ this is only for enhanced mode
	while (SPI1STATbits.SPITBF) {} // FIXME: shouldn't we be checking SPITBF?
#else
	UCA0TXBUF = data;
	/* Note: The MSP430x5xx and MSP430x6xx Family Guide, Rev. O (slau208o)
	 * erronously refers to UCAxSTAT as 'UCAxSTATW'. There is no 'W' suffix
	 * in the preprocessor macros found in the MSP430 development headers.
	 */
	while (UCA0STAT & UCBUSY) {} /* Wait for the send to complete. */
#endif
}
/**
 * Sends a sequence of data over SPI.
 */
void spi_blockingSendSeq(uint8_t const* data, const uint8_t len)
{
	int i;
	for (i = 0; i < len; i++) {
		spi_blockingSendByte(*data++);
	}
}
void spi_blockingReadSeq(uint8_t* reply, const uint8_t len)
{
	int i;
	SPI1STATbits.SPIROV = 0; // If we don't clear this bit and SPI1BUF already
	                         // contained something and wasn't read prior to
	                         // us transmitting a dummy byte, we'll hang forever
	                         // at the while (!SPI1STATbits.SPIRBF) {} loop.
	i = SPI1BUF; // dummy read
	for (i = 0; i < len; i++) {
#if defined(__XC16__)
		SPI1BUF = 0x00;
		while (!SPI1STATbits.SPIRBF) {} // FIXME: shouldn't we be checking SPITBF?
		*reply++ = SPI1BUF;
#else
		spi_blockingSendByte(0xFF);
		*reply++ = UCA0RXBUF;
#endif
	}
}

