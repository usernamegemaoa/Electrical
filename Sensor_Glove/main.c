#include "micro_setup.h"
#include "spi.h"
//#include "AD7147.h"

void main(void) 
{
    delay_ms(500);
    setup_mcu();
    setup_spi();
    setup_AD7147();

    unsigned int device = 0xE417;
    unsigned int blanks = 0x000;
    while(1) 
    {   
        delay_ms(100);
        CS_AD7147 = 0;   //set CS to low (transmit data)
        write_spi(device, blanks);
        CS_AD7147 = 1;   //set CS to low (transmit data)
    } 
    return;
}
