#include "micro_setup.h"
#include "spi.h"
//#include "AD7147.h"

void main(void) 
{
    delay_ms(500);
    setup_spi();
    setup_AD7147();
    TRISD = 0b00000000;         // Port D: Output: bn n 0-7
    TRISE = 0b00000000;
    PORTDbits.RD7 = 1;
    PORTDbits.RD6 = 0;  
    PORTDbits.RD5 = 0;  
    PORTDbits.RD4 = 0;  
    PORTDbits.RD3 = 0;  
    PORTDbits.RD2 = 0;  
    PORTDbits.RD1 = 0;  
    PORTDbits.RD0 = 0;   
    unsigned int bb = 0b1100100011110000;
    while(1) 
    {   
        delay_ms(1);
        //setup_AD7147();
        PORTEbits.RE0 = 0;   //set CS to low (transmit data)
        write_spi(bb);
        write_spi(bb);
        PORTEbits.RE0 = 1;   //set CS to low (transmit data)
    } 
    return;
}
