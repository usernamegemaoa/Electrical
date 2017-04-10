#include "spi.h"
#include "micro_setup.h"

void setup_spi(void) 
{
    SSP1STATbits.SMP = 0;   //SPI Master Mode: Input data sampled at middle of data output time
    SSP1STATbits.CKE = 0;   //Transition occurs on rising edge
    
    SSP1CON1bits.WCOL = 0;  //Collision Detection Bit, 0 = no collision
    SSP1CON1bits.CKP = 1;   //Clock Polarity, Idle State: 1 = (5V), High
    
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

void w32_spi(unsigned int address, unsigned int data)
{
    unsigned int address_low, address_high, data_low, data_high;

    address_low = address & 0xFF;
    address_high = (address >> 8) & 0xFF;
    
    data_low = data & 0xFF;
    data_high = (data >> 8) & 0xFF;;

    SSP1BUF = address_high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p1 = SSP1BUF;
    
    SSP1BUF = address_low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p2 = SSP1BUF;
    
    SSP1BUF = data_high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p3 = SSP1BUF;
    
    SSP1BUF = data_low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p4 = SSP1BUF;
}

void w16_spi(unsigned int address)
{
    unsigned int address_low, address_high; 
    unsigned int p1, p2;
    
    address_low = address & 0xFF;
    address_high = (address >> 8) & 0xFF;
    
    CS_AD7147 = 0;   //set CS to low (transmit data)
    
    SSP1BUF = address_high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;

    CS_AD7147 = 1;   //set CS to low (transmit data)
}

unsigned int r16_spi(void)
{
    unsigned int dummy = 0x000;
    unsigned int p1, p2, message;
    
    CS_AD7147 = 0;   //set CS to low (transmit data)
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = dummy;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    CS_AD7147 = 1;   //set CS to low (transmit data)
    
    message = (p1<<8) + p2;
    
    return message;    
}

unsigned int w16_r16_spi(unsigned int address)
{
    unsigned int address_low, address_high, result;
    unsigned int blank_low = 0x000;
    unsigned int blank_high = 0x000;
    unsigned int p1, p2, p3, p4;
    
    address_low = address & 0xFF;
    address_high = (address >> 8) & 0xFF;

    
    CS_AD7147 = 0;   //set CS to low (transmit data)
    
        
    SSP1BUF = address_high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = blank_high;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = blank_low;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    CS_AD7147 = 1;   //set CS to low (transmit data)
    
    result = (p3 << 8) + p4;
    return result; 
}

/*
void write_spi_register(unsigned int address[], unsigned int NumberOfRegisters, unsigned int data[])
{
    unsigned int address_low;
    unsigned int address_high;
    unsigned int data_low;
    unsigned int data_high;
    unsigned int p1, p2, p3, p4;
    
    for(int i=0; i<NumberOfRegisters; i++)
    {
        address_low = address[i] & 0xFF;
        address_high = (address[i] >> 8);
        data_low = data[i] & 0xFF;
        data_high = (data[i] >> 8); 
        
        SSP1BUF = address_high;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p1 = SSP1BUF;
    
        SSP1BUF = address_low;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p2 = SSP1BUF;
    
        SSP1BUF = data_high;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p3 = SSP1BUF;
    
        SSP1BUF = data_low;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p4 = SSP1BUF;    
    }
}

void w16_r16_spi_loop(unsigned int address[], unsigned int NumberOfRegisters, unsigned int sensor)
{
    //unsigned int address_low;
    //unsigned int address_high;
    unsigned int blank_low = 0x000;
    unsigned int blank_high = 0x000;
    unsigned int p1, p2, p3, p4;
    
    CS_AD7147 = 0;   //set CS to low (transmit data)
    for(int i=0; i<NumberOfRegisters; i++)
    {
        unsigned int address_low = address[i] & 0xFF;
        unsigned int address_high = (address[i] >> 8);
        
        SSP1BUF = address_high;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p1 = SSP1BUF;
    
        SSP1BUF = address_low;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p2 = SSP1BUF;
    
        SSP1BUF = blank_high;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p3 = SSP1BUF;
    
        SSP1BUF = blank_low;  // Command byte (write to pot 0)
        while(!SSP1STATbits.BF); // wait to complete 
        p4 = SSP1BUF;
        
        CDC_RESULT[sensor+1] = (p3 << 8) + p4;
    }
    CS_AD7147 = 1;   //set CS to low (transmit data)
}
*/
