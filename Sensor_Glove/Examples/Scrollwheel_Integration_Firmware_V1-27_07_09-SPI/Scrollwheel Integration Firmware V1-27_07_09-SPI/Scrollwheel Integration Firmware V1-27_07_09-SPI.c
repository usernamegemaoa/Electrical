// ********************************************************************************
// Scrollwheel firmware
// --------------------
//
// Description: This firmware implements a scrollwheel offering 128 absolute positions
//				This firmware also offers joypad functionality using the North, South, 
//				East, West and Centre sensors.
//				The scrollwheel functionnality is in the function UpdateScrollwheel() 
//				and the joypad functionnality is in the function UpdateJoypad().
// Date: 01/11/2005
// Author: ADI
// ********************************************************************************

// ******************************************************************************************************
// Absolute positions: g_ScrollwheelStatus
// =======================================
//												
//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//        |  |  |  |  |  |     |_____________|------> Positions from 0 to 127
//		  |	 |	|  |  |  |--------------------------> 2 fingers touching scrollwheel												
//		  |	 |	|  |  |-----------------------------> Big finger across scrollwheel
//		  |  |	|  |--------------------------------> Go DOWN command
//		  |  |  |-----------------------------------> Go UP command
//		  |	 |--------------------------------------> Tapping bit.									
//  	  |-----------------------------------------> Activation									
//  	  																							
// ******************************************************************************************************
//
// Joypad status: g_ButtonStatus
// =============================
//												
//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//        				   | | | | | | | | | |------> DOWN button
//						   | | | | | | | | |--------> UP button
//						   | | | | | | |------------> LEFT button
//						   | | | | | |--------------> RIGHT button
//						   | | | | |----------------> CENTRE button
//						   | | | |------------------> Go DOWN command
// 						   | | |--------------------> Go UP command
// 						   | |----------------------> Go LEFT command
// 						   |------------------------> Go RIGHT command
//  	  																							
// ******************************************************************************************************



#include <ADUC841.H>
#include <stdio.h>
#include <stdlib.h>
#include "Include\AD7147regMap.h"
#include "Include\General Definitions.h"
#include "Include\Scrollwheel Algorithm Definitions.h"


//---------------------------------
//General definitions
//---------------------------------
#define SIL_ID									0x1470
#define NB_OF_INT								3


//---------------------------------
//Function prototypes
//---------------------------------

//External functions
//------------------
extern void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
extern void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
extern void ConfigAD7147(void);
extern void Wait(WORD value);
extern void InitialiseTouchWheel(void);
extern WORD UpdateScrollwheel(void);
extern WORD UpdateJoypad(void);

//Local functions
//---------------
static int ServiceAD7147Isr(void);
static void ForceCalibration(void);

//---------------------------------
//Global variables
//---------------------------------

//External global variables
//-------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];

//Local global variables
//----------------------
WORD xdata g_ScrollwheelStatus;
WORD xdata g_ButtonStatus;
BYTE xdata InterruptCounterForInitialisation;
BYTE xdata InterruptCounterForThresholdIntMode;


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
	//SPI interface to AD7147
	DOUT=1;				//Set the DOUT pin
	DIN=1;				//Write a "1" to P0.1 so that it becomes an input.
	CLK=1;				//Set the CLK pin
	CS=1;				//Set the CS pin
	ADUC841_INT0=1;		//Makes sure that the INT0 pin on the ADuC841 is be an input.
	
	//-----------------
	//Configure Device
	//-----------------
	ConfigAD7147();
	ForceCalibration();
	
	//Initialise some variables
	//-------------------------
	InterruptCounterForInitialisation = 0;
	
	/*******************************************************************************************/
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	// Enable interrupt after software initialisation. This section cannot be placed anywhere  //
	// else. These instructions must be changed for another host processor.					   //
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	/*******************************************************************************************/
	IT0=0x1;			//The INT0 interrupt is negative edged triggered
	EX0=1;				//Enable external interrupt "0" (AD7147 interrupt)
	EA=1;				//Enable all interrupts	
	while(1)
	{	
		//This is where status variables can be accessed and this is their format.
		// **************************************************************************************
		// Absolute positions: g_ScrollwheelStatus
		// =======================================
		//												
		//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
		//        |  |  |  |  |  |     |_____________|------> Positions from 0 to 127
		//		  |	 |	|  |  |  |--------------------------> 2 fingers touching scrollwheel												
		//		  |	 |	|  |  |-----------------------------> Big finger across scrollwheel
		//		  |  |	|  |--------------------------------> Go DOWN command
		//		  |  |  |-----------------------------------> Go UP command
		//		  |	 |--------------------------------------> Tapping bit.									
		//  	  |-----------------------------------------> Activation									
		//  	  																							
		// **************************************************************************************
		//
		// Joypad status: g_ButtonStatus
		// =============================
		//												
		//	bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
		//        				   | | | | | | | | | |------> DOWN button
		//						   | | | | | | | | |--------> UP button
		//						   | | | | | | |------------> LEFT button
		//						   | | | | | |--------------> RIGHT button
		//						   | | | | |----------------> CENTRE button
		//						   | | | |------------------> Go DOWN command
		// 						   | | |--------------------> Go UP command
		// 						   | |----------------------> Go LEFT command
		// 						   |------------------------> Go RIGHT command
		//  	  																							
		// **************************************************************************************
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
	ReadFromAD7147(STAGE_LOW_LIMIT_INT, 3, AD7147Registers, STAGE_LOW_LIMIT_INT);
	
	if(InterruptCounterForInitialisation<(NB_OF_INT+2))
	{		
		if (InterruptCounterForInitialisation==(NB_OF_INT-1))
		{
			//================================
			//= Put initialisation code here =
			//================================			
			InitialiseTouchWheel();
		}
		InterruptCounterForInitialisation++;
	}
	
	if(InterruptCounterForInitialisation>=NB_OF_INT)
	{
		//========================================================
		//= Recalibrate touchpad if there is a touch on power up =
		//========================================================
		if( ((AD7147Registers[STAGE_HIGH_LIMIT_INT] & POWER_UP_INTERRUPT) == 0x0000) &&
			((AD7147Registers[STAGE_LOW_LIMIT_INT] & POWER_UP_INTERRUPT) != 0x0000))
		{
			ForceCalibration();
			//Initialise again touchpad
			InterruptCounterForInitialisation=0;
		}
		else
		{
			g_ButtonStatus = UpdateJoypad();
			g_ScrollwheelStatus = UpdateScrollwheel();
			//g_ButtonStatus and g_ScrollwheelStatus are updated from this line onwards
		}
		
		//=======================
		// Change interrupt mode
		//=======================
		//Configure AD7147 in EOC interrupt driven mode
		if (((AD7147Registers[STAGE_HIGH_LIMIT_INT] & POWER_UP_INTERRUPT)!=0x0000) || ((AD7147Registers[STAGE_LOW_LIMIT_INT] & POWER_UP_INTERRUPT)!=0x0000))
		{
			if (AD7147Registers[STAGE_COMPLETE_INT_EN]==0x0000)
			{
				AD7147Registers[STAGE_LOW_INT_EN] &= 0xF000;
				AD7147Registers[STAGE_HIGH_INT_EN] &= 0xF000;
				AD7147Registers[STAGE_COMPLETE_INT_EN]=0x0001;
				WriteToAD7147(STAGE_LOW_INT_EN, 3, AD7147Registers, STAGE_LOW_INT_EN);
			}
			InterruptCounterForThresholdIntMode=NUMBER_OF_INTS_BEFORE_THRES_INT_MODE;
		}
		else
		{
			//Configure AD7147 in threshold interrupt driven mode
			if (InterruptCounterForThresholdIntMode>0)
				InterruptCounterForThresholdIntMode--;
			if (AD7147Registers[STAGE_LOW_INT_EN]==0x0000 && InterruptCounterForThresholdIntMode==0)
			{
				AD7147Registers[STAGE_LOW_INT_EN] |= POWER_UP_INTERRUPT;
				AD7147Registers[STAGE_HIGH_INT_EN] |= POWER_UP_INTERRUPT;
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
	//Read back
	ReadFromAD7147(AMB_COMP_CTRL0, 1, AD7147Registers, AMB_COMP_CTRL0);
}

