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

unsigned int spi(unsigned int part1, unsigned int part2, unsigned int part3, unsigned int part4)
{
    PORTEbits.RE0 = 0;   //set CS to low (transmit data)
    
    SSP1BUF = part1;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p1 = SSP1BUF;
    
    SSP1BUF = part2;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p2 = SSP1BUF;
    
        SSP1BUF = part3;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p3 = SSP1BUF;
    
    SSP1BUF = part4;  // Command byte (write to pot 0)
    while(!SSP1STATbits.BF); // wait to complete 
    unsigned int p4 = SSP1BUF;
    
    PORTEbits.RE0 = 1;  //set CS to high
}

unsigned int cdc_signal(char signal, int number)
{
    // 'c' = configuration (Reading)
    // 'r' = result from CDC (Writing)
    if(signal = 'c')
    {
        
        
    }
    if(signal = 'r')
    {
        
    }
    
}
// CW[15:11] = 11100 (Enable Word)
// CW[10] = 0 (R/W) R=1, W=0
// CW[9:0] = [AD9, AD8, AD7, AD6, AD5, AD4, AD3, AD2, AD1, AD0]

// So writing is to configure the control registers for the CDC conversion and interrupts
// Reading is from the registers

//Bank 1
// 0x000: Setup Control []
// 0x001 - 0x004: Calibration and Setup (3 Registers)
// 0x005 - 0x007: Interrupt Enable (3 Registers)
// 0x008 - 0x00A: Interrupt Status (3 Registers)
// 0x00B - 0x016: CDC 16-Bit Conversion Data (12 Registers)
// 0x017: Device ID Register []
// 0x042: Proximity Status Register []

