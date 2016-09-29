#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "blink_led.h"
#include "micro_setup.h"
#include "hall_effect.h"

void led_blink(int led_temp)
{
        
    __delay_ms(2000);
    led_on(led_temp);
    
    __delay_ms(2000);
        led_off(led_temp);
}

void led_blink_time(int led_temp, int time_temp)
{
    int led;
    double time = time_temp*10000000;
    
    if(0==sizeof(time_temp))
    {
        time = 0;
    }
        
    for(int i = 0; i<time;i++)
    {
        led_on(led_temp);
    }
        for(int i = 0; i<time;i++)
    {
        led_off(led_temp);
    }
}

void led_on(int phase)
{
    switch(phase)
    {
        case 0:
            DH3 = LED_ON;
            break;
        case 1:
            DL3 = LED_ON;
            break;
        case 2:
            DH2 = LED_ON;
            break;
        case 3:
            DL2 = LED_ON;
            break;
        case 4:
            DH1 = LED_ON;
            break;
        case 5:
            DL1 = LED_ON; 
            break;
        default:
            break;
    }
}

void led_off(int phase)
{
    switch(phase)
    {
        case 0:
            DH3 = LED_OFF;
            break;
        case 1:
            DL3 = LED_OFF;
            break;
        case 2:
            DH2 = LED_OFF;
            break;
        case 3:
            DL2 = LED_OFF;
            break;
        case 4:
            DH1 = LED_OFF;
            break;
        case 5:
            DL1 = LED_OFF;
            break;
        default:
            break;
    }
}

void hall_LED(void)
{
    if(SENSEA == HALL_ON && SENSEB == HALL_OFF && SENSEC == HALL_OFF) //state: 1,0,0
    {
        DH1 = LED_ON;
        DH2 = LED_OFF;
        DH3 = LED_OFF;
    }
    if(SENSEA == HALL_ON && SENSEB == HALL_ON && SENSEC == HALL_OFF) //state: 1,1,0
    {
        DH1 = LED_ON;
        DH2 = LED_ON;
        DH3 = LED_OFF;
    }
    if(SENSEA == HALL_OFF && SENSEB == HALL_ON && SENSEC == HALL_OFF) //state: 0,1,0
    {
        DH1 = LED_OFF;
        DH2 = LED_ON;
        DH3 = LED_OFF;
    }
    if(SENSEA == HALL_OFF && SENSEB == HALL_ON && SENSEC == HALL_ON) //state: 0,1,1
    {
        DH1 = LED_OFF;
        DH2 = LED_ON;
        DH3 = LED_ON;
    }
    if(SENSEA == HALL_OFF && SENSEB == HALL_OFF && SENSEC == HALL_ON) //state: 0,0,1
    {
        DH1 = LED_OFF;
        DH2 = LED_OFF;
        DH3 = LED_ON;
    }
    if(SENSEA == HALL_ON && SENSEB == HALL_OFF && SENSEC == HALL_ON) // state: 1,0,1
    {
        DH1 = LED_ON;
        DH2 = LED_OFF;
        DH3 = LED_ON;
    }
}

void hall_LED_state()
{
    int state = get_hall_state();
    switch(state)
    {
        case 1: // 1,0,0
            DH1 = LED_ON;
            DH2 = LED_OFF;
            DH3 = LED_OFF;
            break;
        case 2: //state: 1,1,0
            DH1 = LED_ON;
            DH2 = LED_ON;
            DH3 = LED_OFF;
            break;
        case 3: //state: 0,1,0
            DH1 = LED_OFF;
            DH2 = LED_ON;
            DH3 = LED_OFF;
            break;
        case 4: //state: 0,1,1
            DH1 = LED_OFF;
            DH2 = LED_ON;
            DH3 = LED_ON;
            break;
        case 5: //state: 0,0,1
            DH1 = LED_OFF;
            DH2 = LED_OFF;
            DH3 = LED_ON;
            break;
        case 6: // state: 1,0,1
            DH1 = LED_ON;
            DH2 = LED_OFF;
            DH3 = LED_ON;
            break;
        default:
            break;
    }
}
