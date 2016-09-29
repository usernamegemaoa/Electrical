#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "uart.h"
#include "micro_setup.h"


void uart_write_char(char data)
{ 
    while(!TRMT);
    TXREG = data;
}

char uart_tx_empty(void)
{
    return TRMT;    
}

void uart_write_text(char *text)
{
    for(int i=0; text[i]!='\0'; i++)
    {
        uart_write_char(text[i]);
    }
}

char uart_data_ready(void)
{
    return RCIF;
}

char uart_read_char(void)
{
    while(!RCIF);
    return RCREG;
}

void uart_read_text(char *Output, unsigned int length)
{
    unsigned int i;
    for(int i=0; i<length; i++)
    {
        Output[i] = uart_read_char();
    }
}