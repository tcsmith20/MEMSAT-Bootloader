
#include <stdio.h>
#include <stdlib.h>
#include "xc.h"

void ButtonTest(){
TRISEbits.TRISE4 =0 ;
TRISEbits.TRISE3 =1 ;

while(1){
    if(PORTEbits.RE3==1){
        LATEbits.LATE4 =1;
    }
    else{
        LATEbits.LATE4 =0;
    }
}
}