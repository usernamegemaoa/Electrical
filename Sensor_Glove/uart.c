#include "micro_setup.h"
#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned int cdc_signal(char signal, int number)
{
    // 'c' = configuration (Reading)
    // 'r' = result from CDC (Writing)
    if(signal == 'c')
    {
        
        
    }
    if(signal == 'r')
    {
        
    }
   return signal; 
}

void uart_write_char(char data)
{ 
    while(!TRMT2);
    //while(!PIR1bits.TXIF);
    TXREG2 = data;
}

void uart_write_int(int number)
{ 
    while(!TRMT2);
    //while(!PIR1bits.TXIF);
    char temp = number + '0';
    TXREG = temp;
}

void uart_write_unsigned_int(unsigned int number)
{ 
    char char1 = number & 0xFF;
    char char2 = number >> 8;
    
    while(!TRMT2);
    TXREG2 = char1;
    while(!TRMT2);
    TXREG2 = char2;
}

char uart_int_to_string(int number)
{
    char temp = number + '0';
    return temp;
}

void uart_write_string(const char *text)
{
    for(int i=0; i <= strlen(text); i++)
    {
        uart_write_char(text[i]);
    }
}

char uart_tx_empty(void)
{
    return TRMT;    
}


char uart_data_ready(void)
{
    return RCIF;
}

char uart_read_char(void)
{
    while(PIR3bits.RC2IF == 0)
    {
        if(RCSTA2bits.OERR == 1)
        {
            RCSTA2bits.OERR = 0;
            RCSTA2bits.CREN = 0;
            RCSTA2bits.CREN = 1;
        }
    }
    return RCREG2;
}

void uart_read_text(char *Output, unsigned int length)
{
    for(int i=0; i<length; i++)
    {
        Output[i] = uart_read_char();
    }
}