#include <stdio.h>
#include <stdlib.h>
#include <p24FJ256GA110.h>
#include "xc.h"


void TestProgram1(){
    TRISBbits.TRISB2 =0 ;
    TRISGbits.TRISG15 =0 ;
    TRISEbits.TRISE5 = 0;
    
    
        LATBbits.LATB2 = 1;
         LATGbits.LATG15 = 1;
         LATEbits.LATE5 = 1;
       
    
    
    
}
