#include <xc.h>                 // PIC16F917 Configuration Bit Settings
#include <stdio.h>

#include "micro_setup.h"

// Vref- = 0V, Vref+ = 5V, n = 10 bits, Resolution = (Vref+ - Vref-)/(2^n - 1)
// ADU = 0.004887V (Minimum Required Voltage to change bit)

void analog_init(int analog_port)
{
    // A/D Conversion Clock Select Bits
    // <0-3>: 0 Unimplemented
    // <6-4>: ADCS<2:0> Select Clock Ratio
    // 000 = FOSC/2, 001 = FOSC/8, 010 = FOSC/32, x11 = FRC, 100 = FOSC/4
    // X11 = FRC (Internal Oscillator), 101 = FOSC/16, 110 = FOSC/64
    // <3-0>: 0 Unimplemented
    ADCON1 = 0b01100000; // Setting clock to FOSC/64
    
    // Enabling the ADC Module
    //0: ADON = 1 [ADC is Enabled] (ADC Enable Bit)
    //1: Go/Done = 1 [A/D Conversion Cycle in Progress] (A/D Conversion Status Bit) 
    //<4-2>: Analog Channel Select Bits {4,3,2}
    // AN0 = 000{VL1}, AN1 = 001{VL2}, AN2 = 010{VL3}, AN3 = 011{V_Shunt}
    //5: VCFG0 = 0 [Vss] (Voltage Reference Bit)
    //6: VCFG1 = 0 [Vss] (Voltage Reference Bit)
    //7: ADFM = 1 [Right Justified]  (A/D Conversion Result Format Select)
    switch(analog_port)
    {
        case 1: // AN0 = 000{VL1}
           ADCON0 = 0b10000011;  
           break;
        case 2: // AN1 = 001{VL2}
            ADCON0 = 0b10000111;  
            break;
        case 3: // AN2 = 010{VL3}
            ADCON0 = 0b10001011;  
            break;
        case 4: // AN3 = 011{V_Shunt}
            ADCON0 = 0b10001111;  
            break;
        default:
            break;
    }
    while((ADCON0 & 0b00000010)==0b00000010); //Waiting for the conversion to complete
    ADCON0 -= 0b00000001; // Switch off ADC
}

unsigned int get_voltage(int port)
{
    analog_init(port); // Initializing the port for Analog
    unsigned int output = (ADRESH<<8)+ ADRESL; //Return result
    //Shifts the High (2 bits) MSB to the left then adds the 8 LSB to the 10 bit #
    unsigned int voltage = (output/1023) * 5; // Converts ADC unit to Voltage
    return voltage;
}

unsigned int read_current(void)
{
    unsigned int dc_voltage = get_voltage(4); // Getting the Voltage on AN3, V_Shunt 
    unsigned int dc_current = dc_voltage * 50; // Converting Shunt Voltage to Current
    return dc_current;
}

unsigned int read_phase1(void)
{
    unsigned int phase1 = get_voltage(1);
    return phase1;
}

unsigned int read_phase2(void)
{
    unsigned int phase2 = get_voltage(2);
    return phase2;
}

unsigned int read_phase3(void)
{
    unsigned int phase3 = get_voltage(3);
    return phase3;
}
