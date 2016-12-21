#ifndef MICRO_SETUP_H
#define	MICRO_SETUP_H

#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT/T1OSO and RA7/OSC1/CLKIN/T1OSI)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config DEBUG = ON       // In-Circuit Debugger enabled, RB6/ICSPCLK and RB7/ICSPDAT are dedicated to the debugger

#define VL1 PORTAbits.RA0        // Defined Pin RA0 to variable VL1 
#define VL2 PORTAbits.RA1        // Defined Pin RA1 to variable VL2 
#define VL3 PORTAbits.RA2        // Defined Pin RA2 to variable VL3 
#define V_SHUNT PORTAbits.RA3    // Defined Pin RA4 to variable V_SHUNT 

#define SENSEC PORTEbits.RE0     // Defined Pin RE0 to variable SENSEC 
#define SENSEB PORTEbits.RE1     // Defined Pin RE1 to variable SENSEB 
#define SENSEA PORTEbits.RE2     // Defined Pin RE2 to variable SENSEA 

#define H3 PORTCbits.RC0        // Defined Pin RC0 to variable H3 
#define L3 PORTCbits.RC1        // Defined Pin RC1 to variable L3 
#define H2 PORTCbits.RC2        // Defined Pin RC2 to variable H2 
#define L2 PORTCbits.RC3        // Defined Pin RC3 to variable L2 
#define H1 PORTCbits.RC4        // Defined Pin RC4 to variable H1 
#define L1 PORTCbits.RC5        // Defined Pin RC5 to variable L1

#define DH3 PORTDbits.RD0        // Defined Pin RD0 to variable DH3 
#define DL3 PORTDbits.RD1        // Defined Pin RD1 to variable DL3 
#define DH2 PORTDbits.RD2        // Defined Pin RD2 to variable DH2 
#define DL2 PORTDbits.RD3        // Defined Pin RD3 to variable DL2 
#define DH1 PORTDbits.RD4        // Defined Pin RD4 to variable DH1 
#define DL1 PORTDbits.RD5        // Defined Pin RD5 to variable DL1

#define LED_ON (1)               // DIGITAL: Assigning variable LED_ON to the value: 1
#define LED_OFF (0)              // DIGITAL: Assigning variable LED_OFF to the value: 0

#define DUTY_ON (1)               // DIGITAL: Assigning variable DUTY_ON to the value: 1
#define DUTY_OFF (0)              // DIGITAL: Assigning variable DUTY_OFF to the value: 0

#define HALL_ON (1)               // DIGITAL: Assigning variable HALL_ON to the value: 1
#define HALL_OFF (0)              // DIGITAL: Assigning variable HALL_OFF to the value: 0

#define TX PORTCbits.RC6         // Defined Pin RC6 to variable TX
#define RX PORTCbits.RC7         // Defined Pin RC7 to variable RX

#define _XTAL_FREQ 8000000       // 8 MHz External Crystal 

#define BAUDRATE 9600            // 9600 Baud Rate Selected

//#define BUAD_RATE_CALC ((F_CPU/16/BUAD) - 1) 

#include <xc.h> // include processor files - each processor file is guarded. 

extern int init_state;

void setup_mcu(void);

void setup_port(void);

void init_uart(void);

void init_pin(void);

void init_hall(void);

#endif	/* MICRO_SETUP_H */

