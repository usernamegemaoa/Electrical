// This file implements the joypad functionality of the firmware.
// This is the format of the returned variable.
//
// ******************************************************************************************************
//
// Joypad status: ButtonStatus
// ===========================
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

#include "Include\General Definitions.h"
#include "Include\AD7147RegMap.h"
#include "Include\Scrollwheel Algorithm Definitions.h"
#include "Include\Joypad Definitions.h"


//---------------------------------
//Function prototypes
//---------------------------------

//External functions
//------------------

//Local functions
//---------------
WORD UpdateJoypad(void);


//---------------------------------
//Global variables
//---------------------------------

//External global variables
//-------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];

//Local global variables
//----------------------
//Button Joypad Buttons
BYTE xdata TimeForNewPulse;
bit UpdateJoypadUpOnce, UpdateJoypadDownOnce, UpdateJoypadRightOnce,UpdateJoypadLeftOnce;
BYTE xdata UpJoypadUpdateCounter, xdata DownJoypadUpdateCounter,xdata RightJoypadUpdateCounter,xdata LeftJoypadUpdateCounter;
BYTE xdata JoypadTouchDownStatus;
//Variables for the direction buttons and Button/scrollwheel activation mechanic
BYTE xdata NoDisplacementCounter;
BYTE xdata MovedSinceActivationForDirectionButtons;
bit DirectionButtonFound; DirectionButtonDisplayed;
BYTE xdata DirectionButtonSet;
BYTE xdata NumberOfPositionsAllowed;
bit CenterButtonFlag, CenterButtonWasTouched;
WORD xdata ButtonStatus;


WORD UpdateJoypad(void)
{
	/*********************************************************************************************/
	/*																							 */
	/****************************** CODE FOR THE 4 DIRECTION BUTTONS *****************************/
	/*																							 */
	/*********************************************************************************************/
	
	if ((AD7147Registers[STAGE_HIGH_LIMIT_INT] & POWER_UP_INTERRUPT) != 0x0000)
	{
		//*************************************************/
		//* Decode the interrupts bits to set the buttons */
		//*************************************************/
		if((AD7147Registers[STAGE_HIGH_LIMIT_INT] & 0x00FF) != 0x0000)
			JoypadTouchDownStatus = 0x0001;
		else if((AD7147Registers[STAGE_HIGH_LIMIT_INT] & 0x1FF) == 0x0100)
			JoypadTouchDownStatus = 0x0000;
	
		switch (AD7147Registers[STAGE_HIGH_LIMIT_INT] & 0xFF)	//Only look at the scrollwheel bits
		{
			//All possible combinations for top button
			case 0x0001:
				DirectionButtonSet=1;
				break;
			case 0x0080:
				DirectionButtonSet=1;
				break;
		 	case 0x0081:
				DirectionButtonSet=1;
				break;
			case 0x0083:
				DirectionButtonSet=1;
				break;
			case 0x00C1:
				DirectionButtonSet=1;
				break;
			case 0x00C3:
				DirectionButtonSet=1;
				break;
			//All possible combinations for right button
			case 0x0002:
				DirectionButtonSet=2;
				break;
			case 0x0004:
				DirectionButtonSet=2;
				break;
			case 0x0006:
				DirectionButtonSet=2;
				break;
			case 0x0007:
				DirectionButtonSet=2;
				break;
			case 0x000D:
				DirectionButtonSet=2;
				break;
			case 0x000E:
				DirectionButtonSet=2;
				break;
			//All possible combinations for bottom button
			case 0x0008:
				DirectionButtonSet=3;
				break;
			case 0x0010:
				DirectionButtonSet=3;
				break;
			case 0x0018:
				DirectionButtonSet=3;
				break;
			case 0x001C:
				DirectionButtonSet=3;
				break;
			case 0x0038:
				DirectionButtonSet=3;
				break;
			case 0x003C:
				DirectionButtonSet=3;
				break;
			//All possible combinations for left button
			case 0x0020:
				DirectionButtonSet=4;
				break;
			case 0x0040:
				DirectionButtonSet=4;
				break;
		 	case 0x0060:
				DirectionButtonSet=4;
				break;
			case 0x0070:
				DirectionButtonSet=4;
				break;
			case 0x00E0:
				DirectionButtonSet=4;
				break;
			case 0x00F0:
				DirectionButtonSet=4;
				break;
			default:
				DirectionButtonSet=0;
				break;
		}//End switch (AD7147Registers[STAGE_HIGH_LIMIT_INT])

		
		if (MovedSinceActivationForDirectionButtons==0 && CenterButtonWasTouched==0)
		{
			//If we have not moved since the activation of the scrollwheel then 
			//we count for how many interrupts we remained steady.
			if (NoDisplacementCounter<255)
				NoDisplacementCounter++;
			if (NoDisplacementCounter>NB_OF_INT_IN_IDLE_POSITION && DirectionButtonFound==0)
			{
				switch(DirectionButtonSet)
				{
					case 3:
						ButtonStatus = TOP;
						DirectionButtonDisplayed=1;
						DirectionButtonFound=1;
						break;
					case 2:
						ButtonStatus = RIGHT;
						DirectionButtonDisplayed=1;
						DirectionButtonFound=1;
						break;
					case 1:
						ButtonStatus = BOTTOM;
						DirectionButtonDisplayed=1;
						DirectionButtonFound=1;
						break;
					case 4:
						ButtonStatus = LEFT;
						DirectionButtonDisplayed=1;
						DirectionButtonFound=1;						
						break;
				}//End switch(ButtonSet)
			}
			//This is where we clear the 4 direction buttons once in the ButtonStatus.
			if (DirectionButtonFound==1 && DirectionButtonSet==0 && DirectionButtonDisplayed==1)
			{
				DirectionButtonDisplayed=0;
				ButtonStatus &= 0xFFF0;
			}
		}//End if (MovedSinceActivationForDirectionButtons==0)
		else
		{
			ButtonStatus &= 0xFFF0;
			NoDisplacementCounter = 0x0000;
		}//End else if (MovedSinceActivationForDirectionButtons==0)

		// ButtonStatus is organised as followed:
	 	// ========================================
		// bit: 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
		//						 	| |	| | | | | | |
		//						 	| |	| | | | | | |	========== JOYPAD BUTTONS ==========
		//                    	 	| |	| | | | | | |-----> Joypad UP
		//					  		| |	| | | | | |-------> Joypad DOWN
		//				      		| |	| | | | |---------> Joypad LEFT
		//					  	    | |	| | | |-----------> Joypad RIGHT 
		//					  	    | | | | |-------------> Joypad "OK" Centre Button
		//						    | | | | 
		//						    | | | | 			========= DIRECTION COMMANDS ========
		//						    | | | |--------------->	Go UP
		//						    | | |----------------->	Go DOWN
		//						    | |-------------------> Go LEFT
		//						    |---------------------> Go RIGHT			

		/************************************************************************************/
		/* Implement an auto-scrolling if we stay on the lower or upper arrow of the joypad */
		/************************************************************************************/
		if (NoDisplacementCounter>NB_OF_INT_IN_IDLE_POSITION && DirectionButtonFound==1)
		{
			if (DirectionButtonSet==0x01)	  //Up Pulse
			{
				if (UpdateJoypadUpOnce==1)
				{	//Go up
					UpdateJoypadUpOnce=0;
					ButtonStatus &= 0xFE1F;	 //0xFFBF
					ButtonStatus |= 0x0040;				
				}
				else
				{
					ButtonStatus &= 0xFE1F;	 //0xFFBF				
					UpJoypadUpdateCounter++;
					if (UpJoypadUpdateCounter>=TimeForNewPulse)
					{
						UpJoypadUpdateCounter=0;
						UpdateJoypadUpOnce=1;
						TimeForNewPulse=JOYPAD_QUICK_UPDATE;
					}
				}
			}
			else if (DirectionButtonSet==0x0003)	//Down Pulse
			{
				if (UpdateJoypadDownOnce==1)
				{	//Go down
					UpdateJoypadDownOnce=0;
					ButtonStatus &= 0xFE1F;	//0xFFDF;
					ButtonStatus |= 0x0020;
				}
				else
				{
					ButtonStatus &= 0xFE1F;	//0xFFDF;
					DownJoypadUpdateCounter++;
					if (DownJoypadUpdateCounter>=TimeForNewPulse)
					{
						DownJoypadUpdateCounter=0;
						UpdateJoypadDownOnce=1;
						TimeForNewPulse=JOYPAD_QUICK_UPDATE;
					}
				}
			}
			else if (DirectionButtonSet==0x0004)  //Left Pulse
			{
				if (UpdateJoypadLeftOnce==1)
				{	//Go down
					UpdateJoypadLeftOnce=0;
					ButtonStatus &= 0xFE1F;	//0xFF7F;
					ButtonStatus |= 0x0080;
				}
				else
				{
					ButtonStatus &= 0xFE1F;	//0xFF7F;
					LeftJoypadUpdateCounter++;
					if (LeftJoypadUpdateCounter>=TimeForNewPulse)
					{
						LeftJoypadUpdateCounter=0;
						UpdateJoypadLeftOnce=1;
						TimeForNewPulse=JOYPAD_QUICK_UPDATE;
					}
				}
			}
			else if (DirectionButtonSet==0x02)  //Right Pulse
			{
				if (UpdateJoypadRightOnce==1)
				{	//Go down
					UpdateJoypadRightOnce=0;
					ButtonStatus &= 0xFE1F;	//0xFEFF;
					ButtonStatus |= 0x0100;
				}
				else
				{
					ButtonStatus &= 0xFE1F;	//0xFEFF;
					RightJoypadUpdateCounter++;
					if (RightJoypadUpdateCounter>=TimeForNewPulse)
					{
						RightJoypadUpdateCounter=0;
						UpdateJoypadRightOnce=1;
						TimeForNewPulse=JOYPAD_QUICK_UPDATE;
					}
				}
			}
		}
		else
		{
			//Stay static
			UpdateJoypadDownOnce=0;
			UpdateJoypadUpOnce=0;
			UpdateJoypadLeftOnce=0;
			UpdateJoypadRightOnce=0;
			TimeForNewPulse=0;
			UpJoypadUpdateCounter=0;
			DownJoypadUpdateCounter=0;
			LeftJoypadUpdateCounter=0;
			RightJoypadUpdateCounter=0;
			ButtonStatus &= 0xFE1F;
		}		
		
		//**************************/
		//* Code for center button */
		//**************************/
		if (((AD7147Registers[STAGE_HIGH_LIMIT_INT] & 0x0100)==0x0100) && 
			(MovedSinceActivationForDirectionButtons==0))
		{
			ButtonStatus = 0x0010;
			//Only set centre button if we aren't touching any of the scrollwheel
			if(JoypadTouchDownStatus == 0x0000)
			{
				ButtonStatus |= 0x0010;
				//We are touching centre button so make sure we clear direction buttons
				ButtonStatus &=0xFFF0;
			}
			else
				ButtonStatus &=0xFFEF;
			
			CenterButtonFlag=1;
			CenterButtonWasTouched=1;
		}
		else
		{
			ButtonStatus &= 0xFFEF;
			CenterButtonFlag=0;
		}
	}
	else	//Sensor is not touched
	{
		//Variables for the direction buttons and Button/scrollwheel activation mechanic
		NoDisplacementCounter=0;
		DirectionButtonFound=0;
		DirectionButtonDisplayed=0;

		MovedSinceActivationForDirectionButtons=0;
		CenterButtonWasTouched=0;
		
		ButtonStatus = 0;//&= 0xFFE0;
	}
	return (ButtonStatus);
}