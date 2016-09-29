#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"

void read_hall(int *hall_A, int *hall_B, int *hall_C)
{ 
    *hall_A = SENSEA;
    *hall_B = SENSEB;
    *hall_C = SENSEC;
}

int get_hall_state(void)
{
    int hallA;
    int hallB;
    int hallC;
    int state;
    read_hall(&hallA, &hallB, &hallC);
    if(hallA && !hallB && !hallC) //State: 1,0,0
    {
       state = 1;
    }
    if(hallA && hallB && !hallC) //State: 1,1,0
    {
       state = 2;
    }
    if(!hallA && hallB && !hallC) //State: 0,1,0
    {
       state = 3;
    }
    if(!hallA && hallB && hallC) //State: 0,1,1
    {
       state = 4;
    }
    if(!hallA && !hallB && hallC) //State: 0,0,1
    {
       state = 5;
    }
    if(hallA && !hallB && hallC) //State: 1,0,1
    {
       state = 6;
    }
    return state;
}

int get_direction(void)
{
    int direction;
    int current_state = get_hall_state();
    if((init_state < current_state) || (init_state == 6 && current_state == 1)) //CLOCKWISE
    {
        direction = 1; //CLOCKWISE
        DL1 = LED_ON;
        DL2 = LED_OFF;
        DL3 = LED_OFF;
        __delay_ms(100);
    }
    else if((init_state > current_state) || (init_state == 1 && current_state == 6)) //COUNTER-CLOCKWISE
    {
        direction = 2; //COUNTER-CLOCKWISE
        DL1 = LED_OFF;
        DL2 = LED_ON;
        DL3 = LED_OFF;
        __delay_ms(100);
    }
    else
    {
        direction = 0;
        DL1 = LED_OFF;
        DL2 = LED_OFF;
        DL3 = LED_OFF;
    }
    init_state = current_state;
    return direction;        
}

