#include "micro_setup.h"
#include "spi.h"
//#include "AD7147.h"

void main(void) 
{
    delay_ms(500);
    setup_mcu();
    setup_spi();
    //setup_AD7147();

    unsigned int device = 0b1110010000010111;
    //unsigned int device = 0b1110000000010111;
    unsigned int device1 = 0b1101000000100111;
    unsigned int dd = 0b1110010000001011;
    unsigned int power_s = 0x02B0;
    unsigned int power_low = 0x02B2;
    unsigned int power_r = 0xE000;
    unsigned int power_rr = 0xE400;
    unsigned int blanks = 0x000;
    while(1) 
    {   
        delay_ms(100);
        CS_AD7147 = 0;   //set CS to low (transmit data)
        write_spi(device, blanks);
        //write_spi(blanks, blanks);
        CS_AD7147 = 1;   //set CS to low (transmit data)
    } 
    return;
}
