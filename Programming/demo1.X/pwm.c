#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"

void init_pwm(void)
{ 

}

void produce_pwm(int frequency)
{

}
void init_timer (void) 
{
   // Timer 0 initialize  
   OPTION_REG = 0x00 | (1<<7); // | (1<<3);
   // Timer 0 interrupt enable
   // Osc off, Internal clk, On, zero T0IF,
   // Enable global interrupts
   INTCON = (1<<T0IE) | (1<<GIE);
}

void interrupt1(void)
{
   // Timer 0
   if(INTCON & (1<<T0IF))
   { // T0 overflowed ?
      INTCON &= ~(1<<T0IF);   // clear timer0 overflow bit.
      TMR0 = 228;  // 30us
      time++;
      do_pwm();
   }   
}

void do_pwm(void) 
{
   if (time>20) time=0;
   if (time<pwm_val)
      setBit(PORTB,SOFT_PWM_PORTB_PIN);
   else
      resBit(PORTB,SOFT_PWM_PORTB_PIN);
}

/*
 * Timer0
 *  OPTION_REGbits.T0CS = 0;  TMR0 Clock Source Select bit
 *  Internal instruction cycle clock used as source
 *  Internal instruction cycle = (Microcontroller oscillator Frequency)/4
 *  OPTION_REGbits.T0SE = 0;  TMR0 Source Edge Select bit
 *  TMR0 register increments on low-to-high transition on T0CKI pin
 *  NOT IMPLEMENTED SINCE T0SC = 0;
 *  OPTION_REGbits.PSA = 0;   Prescaler Assignment bit
 *  Prescaler is assigned to Timer0 module
 *  OPTION_REGbits.PS = 6;    PS<2:0>: Prescaler Rate Select bits
 *  110 = 1:128
 * 
 * 
 */