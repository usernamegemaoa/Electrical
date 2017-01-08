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

void write_spi(unsigned int address, unsigned int data)
{
    unsigned int address_low = address & 0xFF;
    unsigned int address_high = (address >> 8);
    unsigned int data_low = data & 0xFF;
    unsigned int data_high = (data >> 8);
 
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

void write_spi_setup(unsigned int RegisterAddress, unsigned int data[])
{
    unsigned int address_low0, address_low1, address_low2, address_low3, address_low4, address_low5, address_low6, address_low7;
    unsigned int address_high0, address_high1, address_high2, address_high3, address_high4, address_high5, address_high6, address_high7;
    unsigned int data_low0, data_low1, data_low2, data_low3, data_low4, data_low5, data_low6, data_low7;
    unsigned int data_high0, data_high1, data_high2, data_high3, data_high4, data_high5, data_high6, data_high7;
    unsigned int p1, p2, p3, p4;
    unsigned int enable_word = 0xE000;
    unsigned int address0, address1, address2, address3, address4, address5, address6, address7; 
    
    address0 = enable_word + (RegisterAddress+0);
    address1 = enable_word + (RegisterAddress+1);
    address2 = enable_word + (RegisterAddress+2);
    address3 = enable_word + (RegisterAddress+3);
    address4 = enable_word + (RegisterAddress+4);
    address5 = enable_word + (RegisterAddress+5);
    address6 = enable_word + (RegisterAddress+6);
    address7 = enable_word + (RegisterAddress+7);
    
    //Register 0
    address_low0 = address0 & 0xFF;
    address_high0 = (address0 >> 8);
    data_low0 = data[0] & 0xFF;
    data_high0 = (data[0] >> 8);
    
    //Register 1
    address_low1 = address1 & 0xFF;
    address_high1 = (address1 >> 8);
    data_low1 = data[1] & 0xFF;
    data_high1 = (data[1] >> 8);
    
    //Register 2
    address_low2 = address2 & 0xFF;
    address_high2 = (address2 >> 8);
    data_low2 = data[2] & 0xFF;
    data_high2 = (data[2] >> 8);
    
    //Register 3
    address_low3 = address3 & 0xFF;
    address_high3 = (address3 >> 8);
    data_low3 = data[3] & 0xFF;
    data_high3 = (data[3] >> 8);
    
    //Register 4
    address_low4 = address4 & 0xFF;
    address_high4 = (address4 >> 8);
    data_low4 = data[4] & 0xFF;
    data_high4 = (data[4] >> 8);
    
    //Register 5
    address_low5 = address5 & 0xFF;
    address_high5 = (address5 >> 8);
    data_low5 = data[5] & 0xFF;
    data_high5 = (data[5] >> 8);
    
    //Register 6
    address_low6 = address6 & 0xFF;
    address_high6 = (address6 >> 8);
    data_low6 = data[6] & 0xFF;
    data_high6 = (data[6] >> 8);
    
    //Register 7
    address_low7 = address7 & 0xFF;
    address_high7 = (address7 >> 8);
    data_low7 = data[7] & 0xFF;
    data_high7 = (data[7] >> 8);
    
    //Register 0
    SSP1BUF = address_high0;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low0;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high0;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low0;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 1
    SSP1BUF = address_high1;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low1;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high1;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low1;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 2
    SSP1BUF = address_high2;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low2;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high2;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low2;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 3
    SSP1BUF = address_high3;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low3;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high3;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low3;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 4
    SSP1BUF = address_high4;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low4;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high4;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low4;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 5
    SSP1BUF = address_high5;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low5;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high5;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low5;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 6
    SSP1BUF = address_high6;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low6;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high6;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low6;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
    
    //Register 7
    SSP1BUF = address_high7;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p1 = SSP1BUF;
    
    SSP1BUF = address_low7;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p2 = SSP1BUF;
    
    SSP1BUF = data_high7;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p3 = SSP1BUF;
    
    SSP1BUF = data_low7;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    p4 = SSP1BUF;
}


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

void write_spi_solo(unsigned int message)
{
    PORTEbits.RE0 = 0;   //set CS to low (transmit data)
    unsigned int low = message & 0xFF;
    unsigned int high = (message >> 8);
    
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