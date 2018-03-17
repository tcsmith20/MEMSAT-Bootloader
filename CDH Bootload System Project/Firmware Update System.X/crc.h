/* 
 * File:  crc.h
 * Author: Tanner Smith
 * 
 * Created on March 6, 2018, 9:37 PM
 */

#ifndef CRC_H
#define	CRC_H

#include <stdint.h>

uint16_t CRC_Calculate(uint8_t* data_buffer, uint8_t size, int initial_run, int final_run);


#endif	/* CRC_H */

