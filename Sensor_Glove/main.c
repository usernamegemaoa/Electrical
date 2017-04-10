#include "micro_setup.h"
#include "spi.h"
#include "uart.h"
#include "AD7147.h"

void main(void) 
{
    delay_ms(500);
    setup_mcu();
    setup_spi();
    setup_AD7147();
    
    char indicator;
    unsigned int bb = 1020;
    while(1) 
    {   
        indicator = uart_read_char();
        if (indicator == 'i')
        {
            uart_write_char('i');
            unsigned int device_ID = get_ID();
            uart_write_unsigned_int(device_ID);
        }
        if (indicator == 's')
        {
            uart_write_char('s');
            unsigned int sensor_1 = read_sensor(1);
            uart_write_unsigned_int(sensor_1);
        }
        delay_ms(500);
    } 
    return;
}
