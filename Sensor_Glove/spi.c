#include <xc.h>

#include "spi.h"

void setup_spi(void) 
{
    SSP1STATbits.SMP = 0;   //SPI Master Mode: Input data sampled at middle of data output time
    SSP1STATbits.CKE = 0;   //Transition occurs on rising edge
    
    SSP1CON1bits.WCOL = 0;  //Collision Detection Bit, 0 = no collision
    SSP1CON1bits.CKP = 0;   //Clock Polarity, Idle State: 0 = (0V), low
    
    SSP1CON1bits.SSPM3 = 0; //SSPM3:0 SPI Master Mode, Clock = Fosc/64
    SSP1CON1bits.SSPM2 = 0;
    SSP1CON1bits.SSPM1 = 1;
    SSP1CON1bits.SSPM0 = 0;
    
    PIE1bits.SSP1IE = 0;    // Interrupt is disabled
    SSP1CON1bits.SSPEN = 1; //Enables the ports for SSP
    
    TRISC &= 0xDF;      // clearing TRISC<5> for SDO1
    TRISC &= 0xF7;      // clearing TRISC<3> for SCK1
    // SDI is automatically controlled by the SPI module
    TRISF &= 0x7F;      // clearing TRISC<7> for SS1
}

void write_spi(unsigned int message)
{
    unsigned int high = message & 0xFF;
    unsigned int low = (message >> 8);
    
    SSP1BUF = high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p1 = SSP1BUF;
    
    SSP1BUF = low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p2 = SSP1BUF;
}
void write_spi_solo(unsigned int message)
{
    PORTEbits.RE0 = 0;   //set CS to low (transmit data)
    unsigned int high = message & 0xFF;
    unsigned int low = (message >> 8);
    
    SSP1BUF = high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p1 = SSP1BUF;
    
    SSP1BUF = low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p2 = SSP1BUF;

    PORTEbits.RE0 = 1;  //set CS to high
}


unsigned int read_spi(void)
{
    unsigned int dummy = 0x00;
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int part1 = SSP1BUF;
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int part2 = SSP1BUF;
    
    unsigned int message = (part1<<8) + part2;
    return message;    
}
unsigned int read_spi_solo(void)
{
    PORTEbits.RE0 = 0;   //set CS to low (transmit data)
    
    unsigned int dummy = 0x00;
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int part1 = SSP1BUF;
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int part2 = SSP1BUF;
    
    PORTEbits.RE0 = 1;  //set CS to high
    
    unsigned int message = (part1<<8) + part2;
    return message;    
}