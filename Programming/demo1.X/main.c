#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>
#include <string.h>

#include "blink_led.h"
#include "micro_setup.h"
#include "hall_effect.h"

void main(void) 
{
   setup_mcu();

    while(1) 
    {
        int d = get_direction();
    } 
    return;
}

 