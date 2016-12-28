#include "Include\General Definitions.h"
#include "Include\AD7147RegMap.h"
#include "Include\Scrollwheel Algorithm Definitions.h"



//---------------------------------
//Function prototypes
//---------------------------------
//External functions
extern void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
extern void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);

//Local functions
void ConfigAD7147(void);


//---------------------------------
//Global variables
//---------------------------------

//External global variables 
//---------------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];


//----------------------
//Function declarations
//----------------------
void ConfigAD7147(void);

//---------------------
//Function definitions
//---------------------
void ConfigAD7147(void)
{
	WORD xdata StageBuffer[8];

	//==========================================
	//============ Bank 2 Registers ============
	//==========================================
	
	//========================
	//= Stage 0 CIN3(-) S1   =
	//========================
	StageBuffer[0]=0xFFBF;	//Register 0x80
	StageBuffer[1]=0x1FFF;	//Register 0x81
	StageBuffer[2]=0x0100;	//Register 0x82
	StageBuffer[3]=0x2121;	//Register 0x83
	StageBuffer[4]=4000;	//Register 0x84
	StageBuffer[5]=4000;	//Register 0x85
	StageBuffer[6]=4250;	//Register 0x86
	StageBuffer[7]=4250;	//Register 0x87
	WriteToAD7147(STAGE0_CONNECTION, 8, StageBuffer, 0);
	
	
	//=======================
	//= Stage 1 CIN2(-)  S2 =
	//=======================
	StageBuffer[0]=0xFFEF;	//Register 0x88
	StageBuffer[1]=0x1FFF;	//Register 0x89
	StageBuffer[2]=0x0100;	//Register 0x8A
	StageBuffer[3]=0x2121;	//Register 0x8B
	StageBuffer[4]=4000;	//Register 0x8C
	StageBuffer[5]=4000;	//Register 0x8D
	StageBuffer[6]=4250;	//Register 0x8E
	StageBuffer[7]=4250;	//Register 0x8F
	WriteToAD7147(STAGE1_CONNECTION, 8, StageBuffer, 0);
	
	
	//========================
	//= Stage 2 - CIN1(-) S3 =
	//========================
	StageBuffer[0]=0xFFFB;	//Register 0x90
	StageBuffer[1]=0x1FFF;	//Register 0x91
	StageBuffer[2]=0x0100;	//Register 0x92
	StageBuffer[3]=0x2121;	//Register 0x93
	StageBuffer[4]=4000;	//Register 0x94
	StageBuffer[5]=4000;	//Register 0x95
	StageBuffer[6]=4250;	//Register 0x96
	StageBuffer[7]=4250;	//Register 0x97
	WriteToAD7147(STAGE2_CONNECTION, 8, StageBuffer, 0);
	
	
	//========================
	//= Stage 3 - CIN0(-) S4 =
	//========================
	StageBuffer[0]=0xFFFE;	//Register 0x98
	StageBuffer[1]=0x1FFF;	//Register 0x99
	StageBuffer[2]=0x0100;	//Register 0x9A
	StageBuffer[3]=0x2121;	//Register 0x9B
	StageBuffer[4]=4000;	//Register 0x9C
	StageBuffer[5]=4000;	//Register 0x9D
	StageBuffer[6]=4250;	//Register 0x9E
	StageBuffer[7]=4250;	//Register 0x9F
	WriteToAD7147(STAGE3_CONNECTION, 8, StageBuffer, 0);
		
	
	//========================
	//= Stage 4 - CIN8(-) S5 = 
	//========================
	StageBuffer[0]=0xFFFF;	//Register 0xA0
	StageBuffer[1]=0x2FFB;	//Register 0xA1
	StageBuffer[2]=0x0F00;	//Register 0xA2
	StageBuffer[3]=0x2121;	//Register 0xA3
	StageBuffer[4]=4000;	//Register 0xA4
	StageBuffer[5]=4000;	//Register 0xA5
	StageBuffer[6]=4250;	//Register 0xA6
	StageBuffer[7]=4250;	//Register 0xA7
	WriteToAD7147(STAGE4_CONNECTION, 8, StageBuffer, 0);
	
	
	//=========================
	//= Stage 5 -  CIN7(-) S6 =
	//=========================
	StageBuffer[0]=0xFFFF;	//Register 0xA8
	StageBuffer[1]=0x2FFE;	//Register 0xA9
	StageBuffer[2]=0x0F00;	//Register 0xAA
	StageBuffer[3]=0x2121;	//Register 0xAB
	StageBuffer[4]=4000;	//Register 0xAC
	StageBuffer[5]=4000;	//Register 0xAD
	StageBuffer[6]=4250;	//Register 0xAE
	StageBuffer[7]=4250;	//Register 0xAF
	WriteToAD7147(STAGE5_CONNECTION, 8, StageBuffer, 0);
	
	
	//=========================
	//= Stage 6 -  CIN6(-) S7 =
	//=========================
	StageBuffer[0]=0xEFFF;	//Register 0xB0
	StageBuffer[1]=0x1FFF;	//Register 0xB1	
	StageBuffer[2]=0x0100;	//Register 0xB2
	StageBuffer[3]=0x2121;	//Register 0xB3
	StageBuffer[4]=4000;	//Register 0xB4
	StageBuffer[5]=4000;	//Register 0xB5
	StageBuffer[6]=4250;	//Register 0xB6
	StageBuffer[7]=4250;	//Register 0xB7
	WriteToAD7147(STAGE6_CONNECTION, 8, StageBuffer, 0);
	
	
	//========================
	//= Stage 7 - CIN7(-) S8 =
	//========================
	StageBuffer[0]=0xFBFF;	//Register 0xB8
	StageBuffer[1]=0x1FFF;	//Register 0xB9
	StageBuffer[2]=0x0100;	//Register 0xBA
	StageBuffer[3]=0x2121;	//Register 0xBB
	StageBuffer[4]=4000;	//Register 0xBC
	StageBuffer[5]=4000;	//Register 0xBD
	StageBuffer[6]=4250;	//Register 0xBE
	StageBuffer[7]=4250;	//Register 0xBF
	WriteToAD7147(STAGE7_CONNECTION, 8, StageBuffer, 0);
	
	//====================================
	//= Stage 8 -  CIN4(-) Centre Button =
	//====================================
	StageBuffer[0]=0xFEFF;	//Register 0xC0
	StageBuffer[1]=0x1FFF;	//Register 0xC1
	StageBuffer[2]=0x0100;	//Register 0xC2
	StageBuffer[3]=0x2626;	//Register 0xC3
	StageBuffer[4]=4700;	//Register 0xC4
	StageBuffer[5]=4700;	//Register 0xC5
	StageBuffer[6]=4900;	//Register 0xC6
	StageBuffer[7]=4900;	//Register 0xC7
	WriteToAD7147(STAGE8_CONNECTION, 8, StageBuffer, 0);
	
	
	//===================
	// Stage 9 - Unused =
	//===================
	StageBuffer[0]=0xFFFF;	//Register 0xC8
	StageBuffer[1]=0x3FFF;	//Register 0xC9
	StageBuffer[2]=0x0000;	//Register 0xCA
	StageBuffer[3]=0x2626;	//Register 0xCB
	StageBuffer[4]=4000;	//Register 0xCC
	StageBuffer[5]=2500;	//Register 0xCD
	StageBuffer[6]=4000;	//Register 0xCE
	StageBuffer[7]=4000;	//Register 0xCF
	WriteToAD7147(STAGE9_CONNECTION, 8, StageBuffer, 0);
	

	//=====================
	//= Stage 10 - Unused =
	//=====================
	StageBuffer[0]=0xFFFF;	//Register 0xD0
	StageBuffer[1]=0x3FFF;	//Register 0xD1
	StageBuffer[2]=0x0000;	//Register 0xD2
	StageBuffer[3]=0x2626;	//Register 0xD3
	StageBuffer[4]=4000;	//Register 0xD4
	StageBuffer[5]=2500;	//Register 0xD5
	StageBuffer[6]=4000;	//Register 0xD6
	StageBuffer[7]=4000;	//Register 0xD7
	WriteToAD7147(STAGE10_CONNECTION, 8, StageBuffer, 0);
	
	
	//=====================
	//= Stage 11 - Unused =
	//=====================
	StageBuffer[0]=0xFFFF;	//Register 0xD8
	StageBuffer[1]=0x3FFF;	//Register 0xD9
	StageBuffer[2]=0x0000;	//Register 0xDA
	StageBuffer[3]=0x2626;	//Register 0xDB
	StageBuffer[4]=4000;	//Register 0xDC
	StageBuffer[5]=2500;	//Register 0xDD
	StageBuffer[6]=4000;	//Register 0xDE
	StageBuffer[7]=4000;	//Register 0xDF
	WriteToAD7147(STAGE11_CONNECTION, 8, StageBuffer, 0);
	

	//==========================================
	//============ Bank 1 Registers ============
	//==========================================
	//Initialisation of the first register bank but not the AMB_COMP_CTRL0
	AD7147Registers[PWR_CONTROL]=0x00B2;		//Register 0x00
	WriteToAD7147(PWR_CONTROL, 1, AD7147Registers, PWR_CONTROL);
	
	//Read threshold status registers before enabling interrupts
	ReadFromAD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers, STAGE_LOW_LIMIT_INT);	//Register 0x08, 0x09 and 0x0A
	
	AD7147Registers[AMB_COMP_CTRL0]=0x3230;			//Register 0x02
	AD7147Registers[AMB_COMP_CTRL1]=0x0819;			//Register 0x03
	AD7147Registers[AMB_COMP_CTRL2]=0x0832;			//Register 0x04
	AD7147Registers[STAGE_LOW_INT_EN]=0x0000;		//Register 0x05
	AD7147Registers[STAGE_HIGH_INT_EN]=0x0000;		//Register 0x06
	AD7147Registers[STAGE_COMPLETE_INT_EN]=0x0001;	//Register 0x07
	WriteToAD7147(AMB_COMP_CTRL0, 6, AD7147Registers, AMB_COMP_CTRL0);
	
	//Enable data path for all sequences
	AD7147Registers[STAGE_CAL_EN]=0x0FFF;			//Register 0x01
	WriteToAD7147(STAGE_CAL_EN, 1, AD7147Registers, STAGE_CAL_EN);

	//Read threshold status registers to clear any eventual interrupts
	ReadFromAD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers, STAGE_LOW_LIMIT_INT);	//Register 0x08, 0x09 and 0x0A
}                                                        	