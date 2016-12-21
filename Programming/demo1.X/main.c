#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>
#include <string.h>

#include "analog.h"
#include "blink_led.h"
#include "hall_effect.h"
#include "micro_setup.h"
#include "pwm.h"
#include "uart.h"


void main(void) 
{
    setup_mcu();
    while(1) 
    {   
        uart_write_voltage(1);
        __delay_ms(500);
    } 
    return;
}

 