#include <stdio.h>
#include <stdlib.h>
#include <p24FJ256GA110.h>
#include "xc.h"
uint32_t i;
uint32_t j;
int counter = 0;
void TestProgram3(){
    TRISBbits.TRISB2 =0 ;
    TRISGbits.TRISG15 =0 ;
    TRISEbits.TRISE5 = 0;
    
    for(i=0; i<100000; i++)
{        if(counter % 3000 ==0){
             LATBbits.LATB2 ^= 1;
             counter++;
             for(j=0; j<10000; j++);
        }
         if(counter % 5000 ==0){
             LATGbits.LATG15 ^= 1;
             counter++;
             for(j=0; j<10000; j++);
        }
           if(counter % 7000 ==0){
             LATEbits.LATE5 ^= 1;
             counter++;
             for(j=0; j<10000; j++);
        }
        counter++;
    }
    
    
    
}


