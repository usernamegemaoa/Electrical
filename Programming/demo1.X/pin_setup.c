#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"

void init_pin(void)
{
    H3 = DUTY_OFF;      
    L3 = DUTY_OFF;       
    H2 = DUTY_OFF;      
    L2 = DUTY_OFF;      
    H1 = DUTY_OFF;       
    L1 = DUTY_OFF;       

    DH3 = LED_OFF;
    DL3 = LED_OFF;
    DH2 = LED_OFF;  
    DL2 = LED_OFF; 
    DH1 = LED_OFF;
    DL1 = LED_OFF;
} 
