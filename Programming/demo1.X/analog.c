#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"
#include "analog.h"
// Vref- = 0V, Vref+ = 5V, n = 10 bits, Resolution = (Vref+ - Vref-)/(2^n - 1)
// ADU = 0.004887V (Minimum Required Voltage to change bit)

void analog_init(int analog_port)
{
    //ADCON0-A/D Control Register
    // Enabling the ADC Module
    //0: ADON = 1 [ADC is Enabled] (ADC Enable Bit)
    //1: Go/Done = 1 [A/D Conversion Cycle in Progress] (A/D Conversion Status Bit) 
    //<4-2>: Analog Channel Select Bits {4,3,2}
    // AN0 = 000{VL1}, AN1 = 001{VL2}, AN2 = 010{VL3}, AN3 = 011{V_Shunt}
    //5: VCFG0 = 0 [VDD] (Voltage Reference Bit)
    //6: VCFG1 = 0 [VSS] (Voltage Reference Bit)
    //7: ADFM = 1 [Right Justified]  (A/D Conversion Result Format Select)
    ADCON0bits.VCFG1 = 0; //Setting low voltage reference to VSS
    ADCON0bits.VCFG0 = 0; //Setting High voltage reference to VDD
    ADCON0bits.ADFM = 1; //Right Justified 
    switch(analog_port)
    {
        case 1: // AN0 = 000{VL1}
            ADCON0bits.CHS = 0;  
            break;
        case 2: // AN1 = 001{VL2}
            ADCON0bits.CHS = 1;
            break;
        case 3: // AN2 = 010{VL3}
            ADCON0bits.CHS = 2; 
            break;
        case 4: // AN3 = 011{V_Shunt}
            ADCON0bits.CHS = 3; 
            break;
        default:
            break;
    }
    
    // A/D Conversion Clock Select Bits
    // <0-3>: 0 Unimplemented
    // <6-4>: ADCS<2:0> Select Clock Ratio
    // 000 = FOSC/2, 001 = FOSC/8, 010 = FOSC/32, x11 = FRC, 100 = FOSC/4
    // X11 = FRC (Internal Oscillator), 101 = FOSC/16, 110 = FOSC/64
    // <7>: 0 Unimplemented
    __delay_ms(2);
    ADCON1 = 0b01010000; // Setting clock to FOSC/16
    
    ADCON0bits.ADON = 1;        // turn on the ADC conversion module
}

unsigned int get_analog(int port)
{
    analog_init(port);          // Initializing the port for 10-Bit ADC
    ADCON0bits.GO = 1;	    // initiates ADC on Selected Channel
    while(!ADCON0bits.GO_DONE) continue;    // Wait until the conversion is done
    //unsigned int output = (ADRESH<<4)|ADRESL; //Return 10 bit result for LEFT Justified
    unsigned int output = (ADRESL<<2)|ADRESH; //Return 10 bit result for RIGHT Justified
    return output;
}

unsigned int read_current(void)
{
    unsigned int dc_voltage = get_analog(4); // Getting the Voltage on AN3, V_Shunt 
    unsigned int dc_current = dc_voltage * 50; // Converting Shunt Voltage to Current
    return dc_current;
}

unsigned int read_phase1(void)
{
    unsigned int phase1 = get_analog(1);
    return phase1;
}

unsigned int read_phase2(void)
{
    unsigned int phase2 = get_analog(2);
    return phase2;
}

unsigned int read_phase3(void)
{
    unsigned int phase3 = get_analog(3);
    return phase3;
}
