#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>


void setup_port(void)
{
    TRISA = 0b10001111;         // Port A: Output: 4-6 Input: 0-3, 7
    TRISB = 0b00000000;         // Port B: Output: 
    TRISC = 0b10000000;         // Port C: Output: 0-6 Input: 7
    TRISD = 0b00000000;         // Port D: Output: 0-7
    TRISE = 0b00000111;         // Port E: Output: 3-7 Input: 0-2
    ANSEL = 0b11101111;         // PORT ANSEL: AN(0-3) & AN(5-7)
}                   


