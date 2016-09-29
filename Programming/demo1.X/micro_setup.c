#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"
#include "hall_effect.h"

int init_state = 0;

void setup_mcu(void)
{
    setup_port();
    init_pin();
    init_hall();
}

void setup_port(void)
{
    TRISA = 0b10001111;         // Port A: Output: 4-6 Input: 0-3, 7
    
    TRISB = 0b00000000;         // Port B: Output: 
    
    TRISC = 0b10000000;         // Port C: Output: 0-6 Input: 7
    // RC7=RX-Input, RC6=TX-Output, 
    TRISD = 0b00000000;         // Port D: Output: 0-7
    
    TRISE = 0b00000111;         // Port E: Output: 3-7 Input: 0-2
    
    ANSEL = 0b00001111;         
    // PORT ANSEL: AN(0-3)[VL1, VL2, VL3, V_SHUNT] - Analog
    // PORT ANSEL: AN(5-7) [SENSEA, SENSEB, SENSEC] - Digital   
}                   

void setup_uart(void)
{
    TXSTA = 0bx010-0x0;         // Transmission Register
    // Bit 7=x, CSRC, Using UART (Asynchronous), Don't care about bit
    // Bit 6=0, TX9, 8-Bit transmission
    // Bit 5=1, TXEN, Transmit Enabled
    // Bit 4=0, SYNC, Asynchronous mode
    // Bit 3=-, -, Unimplemented
    // Bit 2=0, BRGH, Baud Rate Select Bit, 0 = Low Speed
    // Bit 1=x, TRMT, Transmit Shift Register Status Bit
    // Bit 0=0, TX9D, No parity bit
    
    RCSTA = 0b10x1x000;         // Receive Register
    // Bit 7=1, SPEN, Serial Port Enabled
    // Bit 6=0, RX9, 8-Bit reception, RX9 = 0
    // Bit 5=x, SREN, Single Receive Enable Bit- Asynchronous Mode- Don't Care
    // Bit 4=1, CREN, Continuous Receive Enable Bit, Asynchronous mode, Receiver Enabled
    // Bit 3=x, ADDEN, Asynchronous mode 8-bit, RX9=0, Don't Care
    // Bit 2=0, FERR, Framing Error Bit, FERR, 0 = No Framing Error
    // Bit 1=0, OERR, Overrun Error Bit, 0 = No Overrun Error has Occurred
    // Bit 0=0, RX9D, Parity Bit, RX9D, 9th Bit of Received Data
}

char init_uart(const long int baudrate)
{
    unsigned int x;
    x = (_XTAL_FREQ - baudrate*16)/(baudrate*64);
    if(x<256)
    {
        SPBRG = x;          // Writing SPBRG Register
        setup_uart();       // Enabling the UART
        return 1;
    }
    return 0;
}

void init_pin(void)
{
    H3 = DUTY_OFF;      // MOSFET High, Phase 3      
    L3 = DUTY_OFF;      // MOSFET Low, Phase 3          
    H2 = DUTY_OFF;      // MOSFET High, Phase 2         
    L2 = DUTY_OFF;      // MOSFET Low, Phase 2         
    H1 = DUTY_OFF;      // MOSFET High, Phase 1          
    L1 = DUTY_OFF;      // MOSFET Low, Phase 1           

    DH3 = LED_OFF;      // LED DH3 = MOSFET High, Phase 3   
    DL3 = LED_OFF;      // LED DL3 = MOSFET Low, Phase 3   
    DH2 = LED_OFF;      // LED DH2 = MOSFET High, Phase 2     
    DL2 = LED_OFF;      // LED DL2 = MOSFET Low, Phase 2    
    DH1 = LED_OFF;      // LED DH1 = MOSFET High, Phase 1   
    DL1 = LED_OFF;      // LED DL1 = MOSFET Low, Phase 1   
} 

void init_hall(void)
{
    init_state = get_hall_state();
    // 
}
