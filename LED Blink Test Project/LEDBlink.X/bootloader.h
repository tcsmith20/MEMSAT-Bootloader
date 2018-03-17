/*
 * File:   bootload.h
 *      Author: Tanner Smith 
 *
 * Created on March 5, 2018, 11:34 AM
 */

#ifndef BOOTLOAD_H
#define	BOOTLOAD_H

#include <xc.h> 

#define ORIGINAL_IMAGE          1
#define CURRENT_IMAGE           2
#define NEW_IMAGE               3

void BOOTLOAD_Load(int image);

#endif	/* BOOTLOAD_H */

