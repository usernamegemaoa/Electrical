//***************************************************************************
//																			
// AD7147 - Slider Demo Firmware											    
// -----------------------------												
// Date: 23/07/2009
// Author: ADI																
//																			
//***************************************************************************
// This firmware implements a slider that offers 128 positions. It requires
// the AD7147 evaluation board to operate. The firmware can be downloaded
// and ran using this board.
//
// This is the format of the status variable returned by the firmware:
//	Global variable "g_SliderStatus":
//  --------------------------------------------------
//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//        |  |  |  |  |  | |_________________|-------> Slider position ranging from 0 to 127
//		  |	 |	|  |  |	 |---------------------------> 2 fingers touching the slider
//		  |	 |	|  |  |------------------------------> Large finger across the slider
//		  |	 |	|  |---------------------------------> Menu DOWN command
//  	  |	 |	|------------------------------------> Menu UP command
//		  |  |---------------------------------------> Tap on the slider
//		  |------------------------------------------> Finger valid	(Activation bit)
//
//***************************************************************************


#include <ADUC841.H>
#include <stdio.h>
#include <stdlib.h>
#include <intrins.h>
#include "Include\General Definitions.h"
#include "Include\AD7147regMap.h"
#include "Include\AD7147 - Slider Definitions.h"
 

//---------------------------------
//Function prototypes
//---------------------------------

//External functions
//---------------------------------
extern void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
extern void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
extern void ConfigAD7147(void);
extern WORD GetNewSliderUpdate(void);
extern void InitialiseSlider(void);


//Local functions
//---------------------------------
static int ServiceAD7147Isr(void);
static void ForceCalibration(void);


//---------------------------------
//Global variables
//---------------------------------
WORD xdata g_SliderStatus;

//External global variables 
//---------------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];


//Local global variables
//---------------------------------
//Initialisation variables
BYTE xdata InterruptCounterForInitialisation;
//Variables for switching from 1 int mode to another one
BYTE xdata InterruptCounterForThresIntMode;

//---------------------------------
//void AD7147_Isr() interrupt 0
//---------------------------------
//Interrupt service routine that is executed when the INT0 pin goes low.
/********************************************************************************************/
/*//////////////////////////////////////////////////////////////////////////////////////////*/
// This function declaration is specific to the ADuC841 and needs to be changed for another //
// host processor. The interrupt vector number may be different 		   					//
/*//////////////////////////////////////////////////////////////////////////////////////////*/
/********************************************************************************************/
static void AD7147_Isr() interrupt 0
{
	EX0=0;
	ServiceAD7147Isr();
	EX0=1;
	return;
}


void main (void)
{
	/********************************************************************************************/
	/*//////////////////////////////////////////////////////////////////////////////////////////*/
	// Specific to the ADuC841, this section configures the ADuC841 pins as input or as output. //
 	// This needs to be changed for another host processor 		   							  	//
	/*//////////////////////////////////////////////////////////////////////////////////////////*/
	/********************************************************************************************/
	//I2C interface to AD7147
	SDA=1;				//Set SDA pin
	CLK=1;				//Set the CLK pin
	ADUC841_INT0=1;		//Makes sure that the INT0 pin on the ADuC841 is be an input.

	//-----------------
	//Configure Device
	//-----------------
	ConfigAD7147();	
	
	//Initialise Calibration variables
	//--------------------------------
	InterruptCounterForThresIntMode=0;
	InterruptCounterForInitialisation=0;
		
	/*******************************************************************************************/
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	// Enable interrupt after software initialisation. This section cannot be placed anywhere  //
	// else. These instructions must be changed for another host processor.					   //
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	/*******************************************************************************************/
	IT0=0x1;			//The INT0 interrupt is negative edged triggered
	EX0=1;				//Enable external interrupt "0" //AD7147 interrupt
	EA=1;				//Enable interrupts
	
	while(1)
	{	
		//This is where the "g_SliderStatus" can be accessed and this is its format.
		//
		//	Global variable "g_SliderStatus":
		//  ------------------------------------------
		//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
		//        |  |  |  |  |  | |_________________|-------> Slider position ranging from 0 to 127
		//		  |	 |	|  |  |	 |---------------------------> 2 fingers touching the slider
		//		  |	 |	|  |  |------------------------------> Large finger across the slider
		//		  |	 |	|  |---------------------------------> Menu DOWN command
		//  	  |	 |	|------------------------------------> Menu UP command
		//		  |  |---------------------------------------> Tap on the slider
		//		  |------------------------------------------> Finger valid	(Activation bit)
		//
	}
}


	
//---------------------------------
//Function  definitions 
//---------------------------------

//ServiceAD7147Isr();
//-----------------------------------------------------------------
// Function called by the AD7147 ISR. Anything that must be 
// executed during the ISR needs to be done here
//-----------------------------------------------------------------
static int ServiceAD7147Isr(void)
{			
	//Read thresholds registers
	ReadFromAD7147(STAGE_LOW_LIMIT_INT, 2, AD7147Registers, STAGE_LOW_LIMIT_INT);	
	
	if(InterruptCounterForInitialisation < (NB_OF_INT+2))
	{		
		if (InterruptCounterForInitialisation==(NB_OF_INT-1))
		{
			//================================
			//= Put initialisation code here =
			//================================
			InitialiseSlider();
		}
		InterruptCounterForInitialisation++;
	}
	
	if(InterruptCounterForInitialisation>=NB_OF_INT)
	{
		//============================
		//= Recalibrate if required. =
		//============================
		if ((AD7147Registers[STAGE_LOW_LIMIT_INT] & POWER_UP_INTERRUPT) != 0x0000)
		{
			ForceCalibration();
			InterruptCounterForInitialisation = 0;
		}
		else
		{
			//Get a new slider position status
			g_SliderStatus = GetNewSliderUpdate(); //"g_SliderStatus" is updated from this line on.
		}	
	
		//=======================
		// Change interrupt mode
		//=======================
		//Configure AD7147 in EOC interrupt driven mode
		if (((AD7147Registers[STAGE_HIGH_LIMIT_INT] & POWER_UP_INTERRUPT)!=0x0000) || 
			((AD7147Registers[STAGE_LOW_LIMIT_INT] & POWER_UP_INTERRUPT)!=0x0000))
		{
			if (AD7147Registers[STAGE_COMPLETE_INT_EN]==0x0000)
			{
				AD7147Registers[STAGE_LOW_INT_EN] &= 0xF000;
				AD7147Registers[STAGE_HIGH_INT_EN] &= 0xF000;
				AD7147Registers[STAGE_COMPLETE_INT_EN]=0x0001;
				WriteToAD7147(STAGE_LOW_INT_EN, 3, AD7147Registers, STAGE_LOW_INT_EN);
			}
			InterruptCounterForThresIntMode = NUMBER_OF_INTS_BEFORE_THRES_INT_MODE;
		}
		else
		{
			//Configure AD7147 in threshold interrupt driven mode
			if (InterruptCounterForThresIntMode>0)
				InterruptCounterForThresIntMode--;
			if ( (AD7147Registers[STAGE_HIGH_LIMIT_INT]==0x0000) && InterruptCounterForThresIntMode==0)
			{
				AD7147Registers[STAGE_LOW_INT_EN] |= POWER_UP_INTERRUPT;
				AD7147Registers[STAGE_HIGH_INT_EN] |= STAGE_HIGH_INT_EN_VALUE;
				AD7147Registers[STAGE_COMPLETE_INT_EN]=0x0000;
				WriteToAD7147(STAGE_LOW_INT_EN, 3, AD7147Registers, STAGE_LOW_INT_EN);
			}
		}
	}//End if(InterruptCounterForInitialisation>=NB_OF_INT)	
}





//ForceCalibration();
//-----------------------------------------------------------------
// Function called by ServiceAD7147Isr when there is a touch on 
// power up or when there is a problem with the calibration.
//-----------------------------------------------------------------
static void ForceCalibration(void)
{
	ReadFromAD7147(AMB_COMP_CTRL0, 1, AD7147Registers, AMB_COMP_CTRL0);
	AD7147Registers[AMB_COMP_CTRL0] |= 0x4000;//Set "forced cal" bit
	WriteToAD7147(AMB_COMP_CTRL0, 1, AD7147Registers, AMB_COMP_CTRL0);
}