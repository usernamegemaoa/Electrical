//#pragma OPTIMIZE (9)
/****************************************************************************/
/*																			*/
/* AD7147 Standard Buttons Software											*/
/* --------------------------												*/
/******************************************************************************/
// Description: In this example, we connected all the CIN inputs to VBias.
//  		 	We enabled the End of Stage Conversion Interrupt and read back 
// 			 	the 12 ADC result registers and the End of Conversion Interrupt
// 				Status register.

// Author: Analog Devices BV
// Date: 19/05/2008
/******************************************************************************/


#include <ADUC841.H>
#include <intrins.h>
#include "AD7147regMap.h"
#include "General Definitions.h"
#include "SPI.h"
#include "SPI - Basic Configuration Config.h"

//---------------------------------
//Function prototypes
//---------------------------------
static int ServiceAD7147Isr(void);


//---------------------------------
//void AD7147_Isr() interrupt 0
//---------------------------------
//Interrupt service routine that is executed when the INT0 pin goes low.
//--------------------------------------------------------------------------------
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
	//---------------------------
	//Device initialisation
	//---------------------------
	/********************************************************************************************/
	/*//////////////////////////////////////////////////////////////////////////////////////////*/
	// Specific to the ADuC841, this section configures the ADuC841 pins as input or as output. //
 	// This needs to be changed for another host processor 		   							  	//
	/*//////////////////////////////////////////////////////////////////////////////////////////*/
	/********************************************************************************************/
	DOUT=1;			//Set the DOUT pin
	CLK=1;			//Set the CLK pin
	ADUC841_INT0=1;	//Makes sure that the INT0 pin on the ADuC841 is be an input.
	
	/********************************************************************************/
	/*//////////////////////////////////////////////////////////////////////////////*/
	// Specific to the ADuC841, this line can be removed for another host processor //
	/*//////////////////////////////////////////////////////////////////////////////*/
	/********************************************************************************/	
	CFG841 |= 1;	//Enable the extended internal RAM		
	
	//-----------------
	//Configure Device
	//-----------------
	ConfigAD7147();	
	
	/*******************************************************************************************/
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	// Enable interrupt after software initialisation. This section cannot be placed anywhere  //
	// else. These instructions must be changed for another host processor.					   //
	/*/////////////////////////////////////////////////////////////////////////////////////////*/
	/*******************************************************************************************/
	IT0=0x1;			//INT0 interrupt is negative edged triggered
	EX0=1;				//Enable external interrupt "0" - AD7147 interrupt
	EA=1;				//Enable all interrupts
	
	while(1);
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
	//Read stage complete registers
	ReadFromAD7147(STAGE_COMPLETE_LIMIT_INT, 1, AD7147Registers, STAGE_COMPLETE_LIMIT_INT);
	
	//Read 12 ADC Values
	ReadFromAD7147(ADCRESULT_S0, 12, AD7147Registers, ADCRESULT_S0);
	//This is how we access the ADC Values
	//AD7147Registers[ADCRESULT_S0] corresponds to ADC Value of Stage 0
	//AD7147Registers[ADCRESULT_S1] corresponds to ADC Value of Stage 1
	//AD7147Registers[ADCRESULT_S2] corresponds to ADC Value of Stage 2
	//AD7147Registers[ADCRESULT_S3] corresponds to ADC Value of Stage 3
	//AD7147Registers[ADCRESULT_S4] corresponds to ADC Value of Stage 4
	//AD7147Registers[ADCRESULT_S5] corresponds to ADC Value of Stage 5
	//AD7147Registers[ADCRESULT_S6] corresponds to ADC Value of Stage 6
	//AD7147Registers[ADCRESULT_S7] corresponds to ADC Value of Stage 7
	//AD7147Registers[ADCRESULT_S8] corresponds to ADC Value of Stage 8
	//AD7147Registers[ADCRESULT_S9] corresponds to ADC Value of Stage 9
	//AD7147Registers[ADCRESULT_S10] corresponds to ADC Value of Stage 10
	//AD7147Registers[ADCRESULT_S11] corresponds to ADC Value of Stage 11
}