#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "uart.h"
#include "micro_setup.h"
#include "hall_effect.h"
#include "analog.h"


void uart_write_char(char data)
{ 
    while(!TRMT);
    //while(!PIR1bits.TXIF);
    TXREG = data;
}

void uart_write_int(int number)
{ 
    while(!TRMT);
    //while(!PIR1bits.TXIF);
    char temp = number + '0';
    TXREG = temp;
}

void uart_write_unsigned_int(unsigned int number)
{ 
    while(!TRMT);
    char char1 = number & 0xFF;
    TXREG = char1;
    __delay_ms(1);
    char char2 = number >> 8;
    TXREG = char2;
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

void uart_write_hall_state(void)
{ 
    char temp = uart_int_to_string(get_hall_state());
    char message[] = "Current Hall State: ";
    uart_write_string(message);
    uart_write_char(temp);
    uart_write_char('\n');
}

void uart_write_direction(void)
{ 
    char direction = uart_int_to_string(get_direction());
    /*switch(temp)
    {
        case 0:
            direction = "Not Moving";
            break;
        case 1:
            direction = "Clockwise";
            break;
        case 2:
            direction = "Counter Clockwise";
            break;
        default:
            direction = "Not Moving";
            break;
    }*/
   
    char message[] = "Current Direction: ";
    uart_write_string(message);
    uart_write_char(direction);
    uart_write_char('\n');
}

void uart_write_voltage(int port)
{
    uart_write_char('v');
    unsigned int voltage = get_analog(port);
    uart_write_unsigned_int(voltage);
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
    while(!RCIF);
    return RCREG;
}

void uart_read_text(char *Output, unsigned int length)
{
    for(int i=0; i<length; i++)
    {
        Output[i] = uart_read_char();
    }
}