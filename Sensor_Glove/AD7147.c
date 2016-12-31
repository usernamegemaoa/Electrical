#include "micro_setup.h"
#include "AD7147.h"
#include "spi.h"

void setup_AD7147(void)
{   
    unsigned int StageBuffer[8];
    
    // BANK 2 REGISTERS CONFIGURATION
    // StageBuffer[0] StageX_Connections[6:0] setup
    // StageBuffer[1] StageX_Connections[12:7] setup
    // StageBuffer[2] StageX_AFE_offset Control 
    // StageBuffer[3] StageX_Sensitivity 
    // StageBuffer[4] StageX_Offset_Low Control 
    // StageBuffer[5] StageX_Offset_High Control 
    // StageBuffer[6] StageX_Offset_High_Clamp Control 
    // StageBuffer[7] StageX_Offset_Low_Clamp Control 
    // WriteToAD7147(STAGEX_CONNECTION, 8, StageBuffer, 0);
    
    //===========================
    //= Stage 0 - CIN0 (+) S0
    //===========================
    StageBuffer[0] = 0x3FFE;	//Register 0x80
    //StageBuffer[1] = 0x1FFF;	//Register 0x81
    //StageBuffer[2] = 0x0100;	//Register 0x82
    //StageBuffer[3] = 0x2121;	//Register 0x83
    //StageBuffer[4] = 4000;      //Register 0x84	
    //StageBuffer[5] = 4000;   	//Register 0x85
    //StageBuffer[6] = 4250;      //Register 0x86	
    //StageBuffer[7] = 4250;      //Register 0x87
    write_AD7147(STAGE0_CONNECTION, 1, StageBuffer);
/*
    //===========================
    //= Stage 1 - CIN1 (+) S1
    //===========================
    StageBuffer[0]=0x3FFB;	//Register 0x88
    StageBuffer[1]=0x1FFF;	//Register 0x89
    StageBuffer[2]=0x0100;	//Register 0x8A
    StageBuffer[3]=0x2121;	//Register 0x8B
    StageBuffer[4]=4000;	//Register 0x8C
    StageBuffer[5]=4000;	//Register 0x8D
    StageBuffer[6]=4250;	//Register 0x8E
    StageBuffer[7]=4250;	//Register 0x8F
	write_AD7147(STAGE1_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 2 - CIN2 (+) S2
    //===========================
    StageBuffer[0]=0x3FEF;	//Register 0x90
    StageBuffer[1]=0x1FFF;	//Register 0x91
    StageBuffer[2]=0x0100;	//Register 0x92
    StageBuffer[3]=0x2121;	//Register 0x93
    StageBuffer[4]=4000;	//Register 0x94
    StageBuffer[5]=4000;	//Register 0x95
    StageBuffer[6]=4250;	//Register 0x96
    StageBuffer[7]=4250;	//Register 0x97
	write_AD7147(STAGE2_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 3 - CIN3 (+) S3
    //===========================
    StageBuffer[0]=0x3FBF;	//Register 0x98
    StageBuffer[1]=0x1FFF;	//Register 0x99
    StageBuffer[2]=0x0100;	//Register 0x9A
    StageBuffer[3]=0x2121;	//Register 0x9B
    StageBuffer[4]=4000;	//Register 0x9C
    StageBuffer[5]=4000;	//Register 0x9D
    StageBuffer[6]=4250;	//Register 0x9E
    StageBuffer[7]=4250;	//Register 0x9F
    write_AD7147(STAGE3_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 4 - CIN4 (+) S4
    //===========================
    StageBuffer[0]=0x3EFF;	//Register 0xA0
    StageBuffer[1]=0x1FFF;	//Register 0xA1
    StageBuffer[2]=0x0100;	//Register 0xA2
    StageBuffer[3]=0x2121;	//Register 0xA3
    StageBuffer[4]=4000;	//Register 0xA4
    StageBuffer[5]=4000;	//Register 0xA5
    StageBuffer[6]=4250;	//Register 0xA6
    StageBuffer[7]=4250;	//Register 0xA7
    write_AD7147(STAGE4_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 5 - CIN5 (+) S5
    //===========================
    StageBuffer[0]=0x3BFF;	//Register 0xA8
    StageBuffer[1]=0x1FFF;	//Register 0xA9
    StageBuffer[2]=0x0100;	//Register 0xAA
    StageBuffer[3]=0x2121;	//Register 0xAB
    StageBuffer[4]=4000;	//Register 0xAC
    StageBuffer[5]=4000;	//Register 0xAD
    StageBuffer[6]=4250;	//Register 0xAE
    StageBuffer[7]=4250;	//Register 0xAF
	write_AD7147(STAGE5_CONNECTION, 8, StageBuffer);
    
	//===========================
    //= Stage 6 - CIN6 (+) S6
    //===========================
    StageBuffer[0]=0x2FFF;	//Register 0xB0
    StageBuffer[1]=0x1FFF;	//Register 0xB1
    StageBuffer[2]=0x0100;	//Register 0xB2
    StageBuffer[3]=0x2121;	//Register 0xB3
    StageBuffer[4]=4000;	//Register 0xB4
    StageBuffer[5]=4000;	//Register 0xB5
    StageBuffer[6]=4250;	//Register 0xB6
    StageBuffer[7]=4250;	//Register 0xB7
    write_AD7147(STAGE6_CONNECTION, 8, StageBuffer);
	
    //===========================
    //= Stage 7 - CIN7 (+) S7
    //===========================
    StageBuffer[0]=0x3FFF;	//Register 0xB8
    StageBuffer[1]=0x1FFE;	//Register 0xB9
    StageBuffer[2]=0x0100;	//Register 0xBA
    StageBuffer[3]=0x2121;	//Register 0xBB
    StageBuffer[4]=4000;	//Register 0xBC
    StageBuffer[5]=4000;  	//Register 0xBD
    StageBuffer[6]=4250;	//Register 0xBE
    StageBuffer[7]=4250;	//Register 0xBF
    write_AD7147(STAGE7_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 8 - CIN8 (+) S8
    //===========================
    StageBuffer[0]=0x3FFF;	//Register 0xC0
    StageBuffer[1]=0x1FFB;	//Register 0xC1
    StageBuffer[2]=0x0100;	//Register 0xC2
    StageBuffer[3]=0x2121;	//Register 0xC3
    StageBuffer[4]=4000;	//Register 0xC4
    StageBuffer[5]=4000;	//Register 0xC5
    StageBuffer[6]=4250;	//Register 0xC6
    StageBuffer[7]=4250;	//Register 0xC7
    write_AD7147(STAGE8_CONNECTION, 8, StageBuffer);

    //===========================
    //= Stage 9 - CIN9 (+) S9
    //===========================
    StageBuffer[0]=0x3FFF;	//Register 0xC8	
    StageBuffer[1]=0x1FEF;	//Register 0xC9
    StageBuffer[2]=0x0100;	//Register 0xCA
    StageBuffer[3]=0x2121;	//Register 0xCB
    StageBuffer[4]=4000;	//Register 0xCC
    StageBuffer[5]=4000;	//Register 0xCD
    StageBuffer[6]=4250;	//Register 0xCE
    StageBuffer[7]=4250;	//Register 0xCF
    write_AD7147(STAGE9_CONNECTION, 8, StageBuffer);

    //============================
    //= Stage 10 - CIN10 (+) S10
    //============================
    StageBuffer[0]=0x3FFF;	//Register 0xD0
    StageBuffer[1]=0x1FBF;	//Register 0xD1
    StageBuffer[2]=0x0100;	//Register 0xD2
    StageBuffer[3]=0x2121;	//Register 0xD3
    StageBuffer[4]=4000;	//Register 0xD4
    StageBuffer[5]=4000;	//Register 0xD5
    StageBuffer[6]=4250;	//Register 0xD6
    StageBuffer[7]=4250;	//Register 0xD7
	write_AD7147(STAGE10_CONNECTION, 8, StageBuffer);

    //============================
    //= Stage 11 - CIN11 (+) S11
    //============================
    StageBuffer[0]=0x3FFF;	//Register 0xD8
    StageBuffer[1]=0x1EFF;	//Register 0xD9
    StageBuffer[2]=0x0100;	//Register 0xDA
    StageBuffer[3]=0x2121;	//Register 0xDB
    StageBuffer[4]=4000;	//Register 0xDC
    StageBuffer[5]=4000;	//Register 0xDD
    StageBuffer[6]=4250;	//Register 0xDE
    StageBuffer[7]=4250;	//Register 0xDF
	write_AD7147(STAGE11_CONNECTION, 8, StageBuffer);
	
    //--------------------------------------------------------------------------//
    //-------------------------Bank 1 Registers---------------------------------//
    //--------------------------------------------------------------------------//
    //Initialization of the first register bank but not the STAGE_CAL_EN
    
    AD7147Registers[PWR_CONTROL] = 0x02B2; 			//Register 0x000
    write_AD7147(PWR_CONTROL, 1, AD7147Registers[PWR_CONTROL]);
	
	//Read high and low limit interrupt status before to enable the interrupts
	//read_AD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers); //Registers 0x08 & 0x09 & 0x0A
    
    AD7147Registers[AMB_COMP_CTRL0] = 0x3230;			//Register 0x002
    AD7147Registers[AMB_COMP_CTRL1] = 0x0419;			//Register 0x003
    AD7147Registers[AMB_COMP_CTRL2] = 0x0832;			//Register 0x004
    // End of Conversion Interrupt Mode
    AD7147Registers[STAGE_LOW_INT_EN] = 0x0000;		//Register 0x005
    AD7147Registers[STAGE_HIGH_INT_EN] = 0x0000;		//Register 0x006
    AD7147Registers[STAGE_COMPLETE_INT_EN] = 0x0001;	//Register 0x007
    write_AD7147(AMB_COMP_CTRL0, 6, AD7147Registers);

    //Enable data path for all sequences and enable calibration on 8 stages
    AD7147Registers[STAGE_CAL_EN] = 0x0FFF;			//Register 0x001
    write_AD7147(STAGE_CAL_EN, 1, AD7147Registers);

	//Read High and Low Limit Status registers to clear INT pin
	//read_AD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers); //Registers 0x08 & 0x09 & 0x0A
    */
}


void write_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int DataBuffer[])
{
	unsigned int ControlValue;
    unsigned int DataValue;
    unsigned int enable_word = 0xE000;

	//Write out the Message in two individual 16 bit unsigned int messages
	for (int i=0; i<NumberOfRegisters; i++)
	{
        //Sends 16-bit Signal containing Enable Word and Register Address
        ControlValue = enable_word + (RegisterAddress+i);
        DataValue = DataBuffer[i];
        
        write_spi(ControlValue);
        //Sends 16-bit Signal Containing Data Value
        write_spi(DataValue);
	}
}
/*
void read_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int *DataBuffer[])
{
	unsigned int ControlValue;
    unsigned int DataValue[]; // = [NumberOfRegisters]
    unsigned int enable_word = 0xE400;
    
	//Write out the control word
	for (int i=0; i<NumberOfRegisters; i++)
	{
        //Sends 16-bit Signal containing Enable Word and Register Address
        ControlValue = enable_word + (RegisterAddress+i);
        write_spi(ControlValue);
        //Sends 16-bit Signal Containing Data Value
        DataBuffer[i] = read_spi();
	}
}
/*
unsigned int read_Sensor(int sensor)
{
	//Read stage complete registers
	read_AD7147(STAGE_COMPLETE_LIMIT_INT, 1, AD7147Registers);
	
	//Read 12 ADC Values
	read_AD7147(ADCRESULT_S0, 12, AD7147Registers);
	//This is how we access the ADC Values
	//AD7147Registers[ADCRESULT_S0] corresponds to ADC Value of Stage 0
    //.....
	//AD7147Registers[ADCRESULT_S11] corresponds to ADC Value of Stage 11
    return AD7147Registers[ADC_RESULT[sensor]];
}

static int ServiceAD7147Isr(void)
{	
    unsigned int POWER_UP_INTERRUPT = 0x01FF;
    unsigned int InterruptCounterForThresholdIntMode;
    unsigned int NUMBER_OF_INTS_BEFORE_THRES_INT_MODE;
	//Read thresholds registers
	read_AD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers);
    //=======================
	// Change interrupt mode
    //=======================
	if (((AD7147Registers[STAGE_HIGH_LIMIT_INT] & POWER_UP_INTERRUPT) != 0x0000) || ((AD7147Registers[STAGE_LOW_LIMIT_INT] & POWER_UP_INTERRUPT) != 0x0000))
	{
        //Configure AD7147 in EOC interrupt mode
		if (AD7147Registers[STAGE_COMPLETE_INT_EN] == 0x0000)
		{
			AD7147Registers[STAGE_LOW_INT_EN] &= 0xF000;
			AD7147Registers[STAGE_HIGH_INT_EN] &= 0xF000;
			AD7147Registers[STAGE_COMPLETE_INT_EN] = 0x0001;
			write_AD7147(STAGE_LOW_INT_EN, 3, AD7147Registers);
		}
	//	InterruptCounterForThresholdIntMode = NUMBER_OF_INTS_BEFORE_THRES_INT_MODE;
	}
	else
	{
		//Configure AD7147 in threshold interrupt driven mode
		if (InterruptCounterForThresholdIntMode > 0)
		{
            InterruptCounterForThresholdIntMode--;
        }
			
        if (AD7147Registers[STAGE_HIGH_INT_EN] == 0x0000 && InterruptCounterForThresholdIntMode == 0)
		{
			AD7147Registers[STAGE_LOW_INT_EN] |= POWER_UP_INTERRUPT;
			AD7147Registers[STAGE_HIGH_INT_EN] |= 0x00FF;
			AD7147Registers[STAGE_COMPLETE_INT_EN] = 0x0000;
			write_AD7147(STAGE_LOW_INT_EN, 3, AD7147Registers);
		}
	}
}

static void ForceCalibration(void)
{
	read_AD7147(AMB_COMP_CTRL0, 1, AD7147Registers);
	AD7147Registers[AMB_COMP_CTRL0] |= 0x4000;//Set "forced cal" bit
	write_AD7147(AMB_COMP_CTRL0, 1, AD7147Registers);
}*/