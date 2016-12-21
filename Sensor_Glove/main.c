#include <xc.h>
#include "micro_setup.h"
#include "spi.h"

void main(void) 
{
    setup_spi();
    TRISD = 0b00000000;         // Port D: Output: 0-7
    TRISE = 0b00000000;
    PORTDbits.RD7 = 1;
    PORTDbits.RD6 = 0;  
    PORTDbits.RD5 = 0;  
    PORTDbits.RD4 = 0;  
    PORTDbits.RD3 = 0;  
    PORTDbits.RD2 = 0;  
    PORTDbits.RD1 = 0;  
    PORTDbits.RD0 = 0;     
    while(1) 
    {   
        //delay_ms(500);  
        //PORTDbits.RD7 = 0;
        delay_ms(1);
        unsigned int signal = 1000; //sending 1
        send_spi(signal);
    } 
    return;
}
