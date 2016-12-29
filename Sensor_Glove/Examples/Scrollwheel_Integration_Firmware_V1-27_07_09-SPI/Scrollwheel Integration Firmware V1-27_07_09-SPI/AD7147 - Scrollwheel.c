// This file implements the scrollwheel functionality of the firmware.
// This is the format of the returned variable.
//
// ******************************************************************************************************
// Absolute positions: ScrollwheelStatus
// =====================================
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


#include <math.h>
#include "Include\General Definitions.h"
#include "Include\AD7147RegMap.h"
#include "Include\Scrollwheel Algorithm Definitions.h"

//---------------------------------
//Function prototypes
//---------------------------------

//External functions
//------------------
extern void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
extern void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);


//Local functions
//---------------
WORD UpdateScrollwheel(void);
BYTE FindHighestAndLowestStagesUsed(const WORD InterruptStatusRegister, const WORD LowestStageOfTheSensor, const WORD HighestStageOfTheSensor, const BYTE NumberOfStagesUsed, const BigFingerLevel, BYTE *LowestSensorTouched, BYTE *HighestSensorTouched);
void InitialiseTouchWheel(void);
DWORD ApplyWeight(const WORD SensorVal, const BYTE Order);

//---------------------------------
//Global variables
//---------------------------------

//External global variables
//-------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];
extern BYTE xdata InterruptCounterForThresholdIntMode;
extern BYTE xdata MovedSinceActivationForDirectionButtons;

//Local global variables
//----------------------
WORD xdata AmbientValues[NB_OF_SENSORS_FOR_TW];		
WORD xdata SensorValues[NB_OF_SENSORS_FOR_TW];
//Variables to find the lowest and the highest sensor touched
BYTE xdata LowestSensorBeingTouched, xdata HighestSensorBeingTouched;
//Variables for activation
BYTE xdata FirstTimeTouchWheelTouched;
BYTE xdata ScrollWheelTouchDownCounter;
bit TouchwheelProperlyTouched;
bit TouchwheelFlag;
//Variables for the tap
BYTE xdata ReturnTappingCounterOnTw;
BYTE xdata NoTouchCounterOnTw;
bit EnableTapDisplayOnTw;
bit TappingFlag;
//Variables to find out which sensor has the highest value.
BYTE xdata SensorWithHighestValue, xdata PreviousSensorWithHighestValue;
BYTE xdata SensorBeforeTheHighest, xdata SensorAfterTheHighest;
BYTE xdata SecondSensorBeforeTheHighest, xdata SecondSensorAfterTheHighest;
//Variables for absolute positions
WORD xdata PositionRatio;
BYTE xdata PositionOffset;
WORD xdata MeanValue, xdata PreviousMeanValue;
int xdata MeanValueWithoutOffset, xdata PreviousMeanValueWithoutOffset;
WORD xdata PositionOnTw;
WORD xdata PreviousAbsPosOnTw, xdata AbsPosOnTw;
WORD xdata AvgPositionOnTw;
//Variables for relative positions
WORD xdata DisplayItemsResolution;
int xdata TwelveOclockDisplacement;
int xdata RelativeDisplacement, xdata RelativeDisplacementDebug;
BYTE xdata TwelveOclockPosition;
BYTE xdata TwelveOclockDispRelatedToFirstTouch;
BYTE xdata TwelveOclockPoint;
BYTE xdata DispRelatedToFirstTouch;
BYTE xdata PositionOnFirstTouchForActivation;
BYTE xdata TwelveOclockPointFound;
BYTE xdata PositionOnFirstTouch;
bit DisplacementDetectedSinceActivation;
bit UpdateListBoxDownOnce, UpdateListBoxUpOnce;
bit MovementDetected;
//Fast scroll
WORD xdata PositionOnFirstTouchFastScroll;
BYTE xdata NumberOfUpdates;
BYTE xdata FastScrollUpdateCounter;
BYTE xdata FastScrollDirection;
BYTE xdata MinimalNumberOfInterruptsAfterLiftingOff;
bit FastScrollDetected;
bit RecordSwCounterOnce;
WORD xdata ScrollwheelStatus;



WORD UpdateScrollwheel(void)
{
	BYTE i;
	BYTE xdata TouchError;
	int xdata tempResult;
	WORD xdata AmbientValueAddress;
	DWORD xdata WeightedSensorValues[NB_OF_VALUES_IN_CALCULATIONS];	
	DWORD xdata MeanValueNum;
	WORD xdata MeanValueDen;
	
	
	//= Find which stages have an interrupt set =
	//===========================================
	TouchError=FindHighestAndLowestStagesUsed(AD7147Registers[STAGE_HIGH_LIMIT_INT], LOWER_SENSOR_STAGE, HIGHER_SENSOR_STAGE, 
								  			 NB_OF_SENSORS_FOR_TW, NUMBER_OF_SENSORS_ON_FOR_BIG_FINGER,
											 &LowestSensorBeingTouched, &HighestSensorBeingTouched);
	
	//Read from scrollwheel if activated
	//==================================	
	//if(((AD7147Registers[STAGE_HIGH_LIMIT_INT] & NUMBER_OF_STAGES_BEING_USED) != 0x0000))
	{
		ReadFromAD7147(ADCRESULT_S0, NB_OF_SENSORS_FOR_TW, AD7147Registers, ADCRESULT_S0);
		
		AmbientValueAddress=STAGE0_AMBIENT;
		for(i=0;i<NB_OF_SENSORS_FOR_TW;i++)
		{
			ReadFromAD7147(AmbientValueAddress, 1, AmbientValues, i);
			AmbientValueAddress=AmbientValueAddress+NB_OF_REGS_PER_STAGE;
			
			if (AD7147Registers[ADCRESULT_S0+i] < AmbientValues[i])
				SensorValues[i] = 0;
			else
				SensorValues[i] = AD7147Registers[ADCRESULT_S0+i] - AmbientValues[i];
		}
	}
	
	//= Touchwheel activation =
	//=========================
	if(((AD7147Registers[STAGE_HIGH_LIMIT_INT] & NUMBER_OF_STAGES_BEING_USED) != 0x0000))
	{
		TappingFlag=0;//Touching therefore clear the tapping flag.
		RecordSwCounterOnce =0;
		//Touchwheel touched for the first time
		if (FirstTimeTouchWheelTouched==0)
			FirstTimeTouchWheelTouched=1;
		
		//Check if the user left enough time between 2 taps.
		if (NoTouchCounterOnTw > T_MAX_NO_TOUCHING)
			ReturnTappingCounterOnTw = DISPLAY_AFTER_A_TAP; //Reset counter for displaying the tap
		else
			ReturnTappingCounterOnTw = 0;
		
		if (ScrollWheelTouchDownCounter<26)
		{
			if (ScrollWheelTouchDownCounter >=13)	//13 interrupts @ 40ms between each makes 0.5sec
				TouchwheelProperlyTouched=1;		//Set a flag once we are touching for longer than 0.5sec
			
			if (ScrollWheelTouchDownCounter >1)
				TouchwheelFlag=1;
			
			//Touching for more than 0.5sec, hence clear the NoTouchCounter
			if (ScrollWheelTouchDownCounter > (T_MIN_TOUCHING+T_MAX_TOUCHING))
				NoTouchCounterOnTw = 0;
			ScrollWheelTouchDownCounter++;
		}
		else
		{
			NoTouchCounterOnTw = 0;
			ScrollWheelTouchDownCounter = 255;
		}
	}
	else if((AD7147Registers[STAGE_HIGH_LIMIT_INT] & NUMBER_OF_STAGES_BEING_USED) == 0x0000)
	{	
		TouchwheelFlag=0;
		FirstTimeTouchWheelTouched=0;
		TouchwheelProperlyTouched=0;
		
		//Relative positions
		TwelveOclockPosition = NUMBER_OF_POSITIONS_WANTED;
		TwelveOclockDispRelatedToFirstTouch = 0;
		DisplacementDetectedSinceActivation = 0;
		TwelveOclockPoint = 0;
		DispRelatedToFirstTouch = 0;
				
		//= Fast scroll =
		//===============
		if (RecordSwCounterOnce==1)
		{
			RecordSwCounterOnce =0;
			if (ScrollWheelTouchDownCounter > T_MIN_TOUCHING && ScrollWheelTouchDownCounter < (T_MAX_TOUCHING+18))
			{
				//NumberOfUpdates = abs(PositionOnFirstTouchFastScroll - YAxisPosition) / DisplayItemsResolutionOnY;
				NumberOfUpdates =0;
				RelativeDisplacementDebug = RelativeDisplacement;
				//Check if there will be enough interrupts after lifting off before to switch to threshold mode.
				MinimalNumberOfInterruptsAfterLiftingOff = NumberOfUpdates * LISTBOX_QUICK_UPDATE;
				if ((MinimalNumberOfInterruptsAfterLiftingOff) > NUMBER_OF_INTS_BEFORE_THRES_INT_MODE)
					InterruptCounterForThresholdIntMode = MinimalNumberOfInterruptsAfterLiftingOff + 10;
				
				if (NumberOfUpdates>0)
					FastScrollDetected=1;
				
				if (MovementDetected==0)
				{
					NumberOfUpdates=0;
					FastScrollDetected=0;
				}
			}
			else
			{
				FastScrollDetected=0;
				NumberOfUpdates=0;
			}
		}
		
		//= Work out the tap =
		//====================
		if (NoTouchCounterOnTw < 26)
			NoTouchCounterOnTw++;

		if (ScrollWheelTouchDownCounter > T_MIN_TOUCHING && ScrollWheelTouchDownCounter < T_MAX_TOUCHING &&
			NoTouchCounterOnTw > ScrollWheelTouchDownCounter)
		{				
			//Set a bit for a certain number of interrupts so that the user applications sees it.
			//This is done with a decounter that is reloaded when we are touching
			if (ReturnTappingCounterOnTw > 0)//&& EnableTapDisplayOnTw == 1)
			{
				ReturnTappingCounterOnTw--;
				TappingFlag = 1;	//Tap detected here
			}
			else
			{
				ScrollWheelTouchDownCounter = 0;
				TappingFlag = 0;
			}
		}
		else
		{
			ScrollWheelTouchDownCounter = 0;
			ReturnTappingCounterOnTw = 0;
			TappingFlag = 0;//Cleared tapping bit after lift off 
		}
		
		MovementDetected = 0;
		RelativeDisplacement = 0;
	} 
	
	
	if (TouchwheelFlag==1)
	{
		//= Peak detector that finds the sensor that has the highest response 
		//===================================================================
		PreviousSensorWithHighestValue = SensorWithHighestValue;
		SensorWithHighestValue = 0;
		for (i=0;i<NB_OF_SENSORS_FOR_TW;i++)
		{
			if (i<(NB_OF_SENSORS_FOR_TW-1))
			{
				if((SensorValues[i]>SensorValues[i+1]) && (SensorValues[i]>SensorValues[SensorWithHighestValue]))
					SensorWithHighestValue=i;
			}
			else if (i==(NB_OF_SENSORS_FOR_TW-1))
			{
				if((SensorValues[i]>SensorValues[0]) && (SensorValues[i]>SensorValues[SensorWithHighestValue]))
					SensorWithHighestValue=i;
			}
		}			
		
		//Identify sensors on each side of the one with the highest response
		switch (SensorWithHighestValue)
		{
			case 0:
				SecondSensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-2);
				SensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-1);
				SensorAfterTheHighest=1;
				SecondSensorAfterTheHighest=2;
				break;
			case 1:
				SecondSensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-1);
				SensorBeforeTheHighest=0;
				SensorAfterTheHighest=2;
				SecondSensorAfterTheHighest=3;
				break;
			case (NB_OF_SENSORS_FOR_TW-2):
				SecondSensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-4);
				SensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-3);
				SensorAfterTheHighest=(NB_OF_SENSORS_FOR_TW-1);
				SecondSensorAfterTheHighest=0;
				break;
			case (NB_OF_SENSORS_FOR_TW-1):
				SecondSensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-3);
				SensorBeforeTheHighest=(NB_OF_SENSORS_FOR_TW-2);
				SensorAfterTheHighest=0;
				SecondSensorAfterTheHighest=1;
				break;
			
			default:
				SecondSensorBeforeTheHighest=SensorWithHighestValue-2;
				SensorBeforeTheHighest=SensorWithHighestValue-1;
				SensorAfterTheHighest=SensorWithHighestValue+1;
				SecondSensorAfterTheHighest=SensorWithHighestValue+2;
				break;
		}
		
		//********************************************************************************************
		//******************* CODE FOR THE ABSOLUTE DISPLACEMENT ON THE SCROLLWHEEL ******************
		//********************************************************************************************
		
		if((TouchError & 0x01)!=0x01)
		{
				//*********************************
			//* Calculation of the mean value *
			//*********************************
			//Modify the weights around 12.00 o'clock
			switch (SensorWithHighestValue)
			{
				case 0:	//Sensor 0 has the highest reponse
					WeightedSensorValues[0] = ApplyWeight(SensorValues[SecondSensorBeforeTheHighest], 1);
					WeightedSensorValues[1] = ApplyWeight(SensorValues[SensorBeforeTheHighest], 2);
					WeightedSensorValues[2] = ApplyWeight(SensorValues[SensorWithHighestValue], 3);
					WeightedSensorValues[3] = ApplyWeight(SensorValues[SensorAfterTheHighest], 4);
					WeightedSensorValues[4] = ApplyWeight(SensorValues[SecondSensorAfterTheHighest], 5);
					break;
					
				case 1:	//Sensor 1 has the highest reponse
					WeightedSensorValues[0] = ApplyWeight(SensorValues[SecondSensorBeforeTheHighest], 2);
					WeightedSensorValues[1] = ApplyWeight(SensorValues[SensorBeforeTheHighest], 3);
					WeightedSensorValues[2] = ApplyWeight(SensorValues[SensorWithHighestValue], 4);
					WeightedSensorValues[3] = ApplyWeight(SensorValues[SensorAfterTheHighest], 5);
					WeightedSensorValues[4] = ApplyWeight(SensorValues[SecondSensorAfterTheHighest], 6);
					break;
					
				case (NB_OF_SENSORS_FOR_TW-2):	//Sensor 6 has the highest reponse
					WeightedSensorValues[0] = ApplyWeight(SensorValues[SecondSensorBeforeTheHighest], 7);
					WeightedSensorValues[1] = ApplyWeight(SensorValues[SensorBeforeTheHighest], 8);
					WeightedSensorValues[2] = ApplyWeight(SensorValues[SensorWithHighestValue], 9);
					WeightedSensorValues[3] = ApplyWeight(SensorValues[SensorAfterTheHighest], 10);
					WeightedSensorValues[4] = ApplyWeight(SensorValues[SecondSensorAfterTheHighest], 11);
					break;
					
				case (NB_OF_SENSORS_FOR_TW-1):	//Sensor 7 has the highest reponse
					WeightedSensorValues[0] = ApplyWeight(SensorValues[SecondSensorBeforeTheHighest], 8);
					WeightedSensorValues[1] = ApplyWeight(SensorValues[SensorBeforeTheHighest], 9);
					WeightedSensorValues[2] = ApplyWeight(SensorValues[SensorWithHighestValue], 10);
					WeightedSensorValues[3] = ApplyWeight(SensorValues[SensorAfterTheHighest], 11);
					WeightedSensorValues[4] = ApplyWeight(SensorValues[SecondSensorAfterTheHighest], 12);
					break;
					
				default:
					WeightedSensorValues[0] = ApplyWeight(SensorValues[SecondSensorBeforeTheHighest], (SecondSensorBeforeTheHighest +3));
					WeightedSensorValues[1] = ApplyWeight(SensorValues[SensorBeforeTheHighest], (SensorBeforeTheHighest +3));
					WeightedSensorValues[2] = ApplyWeight(SensorValues[SensorWithHighestValue], (SensorWithHighestValue +3));
					WeightedSensorValues[3] = ApplyWeight(SensorValues[SensorAfterTheHighest], (SensorAfterTheHighest +3));
					WeightedSensorValues[4] = ApplyWeight(SensorValues[SecondSensorAfterTheHighest], (SecondSensorAfterTheHighest +3));
					break; 
			}//End switch (SensorWithHighestValue)
			
			//Keep a copy of the mean value from the previous interrupt.
			PreviousMeanValue = MeanValue;
			
			//Calculate Mean value
			MeanValueNum=0;
			for(i=0; i<NB_OF_VALUES_IN_CALCULATIONS; i++)
				MeanValueNum = MeanValueNum + WeightedSensorValues[i];

			MeanValueDen = SensorValues[SecondSensorBeforeTheHighest] + 
						   SensorValues[SensorBeforeTheHighest] + 
						   SensorValues[SensorWithHighestValue] + 
						   SensorValues[SensorAfterTheHighest] + 
						   SensorValues[SecondSensorAfterTheHighest];
			MeanValueNum /= MeanValueDen;
			MeanValue = MeanValueNum;
			
			
			//= Calculate the offset =
			//========================
			if(TouchwheelProperlyTouched==1)
			{
				if (PreviousSensorWithHighestValue == (NB_OF_SENSORS_FOR_TW-1) && SensorWithHighestValue==0)
					PositionOffset = MeanValue;
				else if (PreviousSensorWithHighestValue == 0 && SensorWithHighestValue == (NB_OF_SENSORS_FOR_TW-1))
					PositionOffset = PreviousMeanValue;
				//Clamp the offset if it goes to high
				if(PositionOffset > OFFSET_POSITION_CLAMP)
					PositionOffset = OFFSET_POSITION_CLAMP;
			}
			
			//* Update mean value fifo =
			//==========================
			//Calculate the mean value without the offset
			PreviousMeanValueWithoutOffset = MeanValueWithoutOffset;
			MeanValueWithoutOffset = MeanValue - PositionOffset;
			
			if(MeanValueWithoutOffset < 0)
				MeanValueWithoutOffset = 0;
			
			//= Calculate ratio to scale down to NUMBER_OF_WANTED_POSITIONS =
			//===============================================================
			if(TouchwheelProperlyTouched==1)
			{
				//Update the ratio
				//Here we multiply the numerator by 100 to avoid using "float" variables.
				if (PreviousSensorWithHighestValue == (NB_OF_SENSORS_FOR_TW-1) && SensorWithHighestValue==0)
					PositionRatio = (PreviousMeanValueWithoutOffset * 100) / NUMBER_OF_POSITIONS_WANTED;
				else if (PreviousSensorWithHighestValue==0 && SensorWithHighestValue == (NB_OF_SENSORS_FOR_TW-1))
					PositionRatio = (MeanValueWithoutOffset * 100) / NUMBER_OF_POSITIONS_WANTED;
			}
			PositionOnTw = (MeanValueWithoutOffset * 100) / PositionRatio;
			if(PositionOnTw > NUMBER_OF_POSITIONS_WANTED)
				PositionOnTw = NUMBER_OF_POSITIONS_WANTED;
			
						
			//= Record position on first touch and keep track of positions =
			//==============================================================
			if(FirstTimeTouchWheelTouched==1)
			{
				FirstTimeTouchWheelTouched=2;
				AvgPositionOnTw = PositionOnTw;
				PreviousAbsPosOnTw = PositionOnTw;
				AbsPosOnTw = PositionOnTw;
				PositionOnFirstTouch = PositionOnTw;
				PositionOnFirstTouchForActivation = PositionOnTw;
				PositionOnFirstTouchFastScroll = PositionOnTw;
			}
			else if(FirstTimeTouchWheelTouched==2)
			{
				//Update position tracking
				PreviousAbsPosOnTw = AbsPosOnTw;
				AbsPosOnTw = PositionOnTw;
				
				if ((PreviousSensorWithHighestValue==(NB_OF_SENSORS_FOR_TW-1) && SensorWithHighestValue==0) ||
					(PreviousSensorWithHighestValue==0 && SensorWithHighestValue==(NB_OF_SENSORS_FOR_TW-1)))
					AvgPositionOnTw = PositionOnTw;
				else
				{
					AvgPositionOnTw = AvgPositionOnTw*30 + PositionOnTw*71;
					AvgPositionOnTw /= 100;
				}
				
				if(AvgPositionOnTw > (NUMBER_OF_POSITIONS_WANTED))
					AvgPositionOnTw = (NUMBER_OF_POSITIONS_WANTED);
			}
			
			//= Find the 12 o'clock absolute position =
			//=========================================
			if (TouchwheelProperlyTouched==1)
			{
				if (PreviousSensorWithHighestValue==(NB_OF_SENSORS_FOR_TW-1) && SensorWithHighestValue==0)
				{
					TwelveOclockPosition = PreviousAbsPosOnTw;
					TwelveOclockDispRelatedToFirstTouch = PreviousAbsPosOnTw;
				}
				else if (PreviousSensorWithHighestValue==0 && SensorWithHighestValue==(NB_OF_SENSORS_FOR_TW-1))
				{
					TwelveOclockPosition = AbsPosOnTw;
					TwelveOclockDispRelatedToFirstTouch = AbsPosOnTw;
				}
			}

			//= Calculate the displacement related to the first touch =
			//=========================================================
			if (DisplacementDetectedSinceActivation==0)
			{
				if (PreviousAbsPosOnTw>96 && AbsPosOnTw<32)//Clockwise
					TwelveOclockPoint=1;
				else if (AbsPosOnTw>96 && PreviousAbsPosOnTw<32)//Anticlockwise
					TwelveOclockPoint=2;
				
				switch (TwelveOclockPoint)
				{
					case 0:
						DispRelatedToFirstTouch=abs(AbsPosOnTw-PositionOnFirstTouchForActivation);
						break;
					case 1:
						DispRelatedToFirstTouch=TwelveOclockDispRelatedToFirstTouch+AbsPosOnTw;
						break;
					case 2:
						DispRelatedToFirstTouch=TwelveOclockDispRelatedToFirstTouch+(NUMBER_OF_POSITIONS_WANTED-AbsPosOnTw);
						break;
				}
				if (DispRelatedToFirstTouch > NUMBER_OF_POSITIONS_TO_ACTIVATE_LISTBOX)
				{
					DisplacementDetectedSinceActivation=1;
					MovedSinceActivationForDirectionButtons = 1;
				}
			}

			//******************** CODE FOR THE RELATIVE DISPLACEMENT FOR THE LISTBOX ********************
			//********************************************************************************************
			if(DisplacementDetectedSinceActivation==1)
			{
				//Catch 12.00 point
				if (PreviousAbsPosOnTw>96 && AbsPosOnTw<32)
				{
					//Clockwise
                    TwelveOclockDisplacement = NUMBER_OF_POSITIONS_WANTED - PreviousAbsPosOnTw + AbsPosOnTw;
                    TwelveOclockPointFound = 1;
				}
				else if (AbsPosOnTw>96 && PreviousAbsPosOnTw<32)
				{
					//Anticlockwise
                    TwelveOclockDisplacement = -(NUMBER_OF_POSITIONS_WANTED - AbsPosOnTw + PreviousAbsPosOnTw);
                    TwelveOclockPointFound = 2;
				}
					
				//Once we've caught the 12.00 point, we make sure that the relative position is greater than 
        	    //SCROLLING_RESOLUTION. If it is not, we keep looking at the absolute position until the 
				//relative displacement reaches SCROLLING_RESOLUTION Once it does, we clear all the flags.
				if (TwelveOclockPointFound==1)
				{
					//Clockwise
	                if (abs(TwelveOclockDisplacement<DisplayItemsResolution))
					{
						//Calculate relative displacement at 12.00
	                    RelativeDisplacement = TwelveOclockDisplacement + AbsPosOnTw;
						//Relative disp at 12.00 is smaller than the scrolling resolution
						//therefore keep calculating the relative displacement until it reaches 
						//the scrolling resolution
	                    if (abs(RelativeDisplacement)>=DisplayItemsResolution)
						{
	                        TwelveOclockPointFound=0;
	                        TwelveOclockDisplacement=0;
	                    }
	                }
					else
					{
						//Relative disp at 12.00 is greater than the scrolling resolution
						//therefore clear all the flags
	                    TwelveOclockPointFound=0;
	                    RelativeDisplacement=TwelveOclockDisplacement;
	                    TwelveOclockDisplacement=0;
	                }
	            }
	            else if (TwelveOclockPointFound==2)
				{
					//Antilockwise
	                if (abs(TwelveOclockDisplacement) < DisplayItemsResolution)
					{
						//Calculate relative displacement at 12.00
						RelativeDisplacement=TwelveOclockDisplacement-(NUMBER_OF_POSITIONS_WANTED - AbsPosOnTw);
	                    if (abs(RelativeDisplacement) >= DisplayItemsResolution)
						{
	                        TwelveOclockPointFound=0;
	                        TwelveOclockDisplacement=0;
	                    }
					}
	                else
					{
						//Relative disp at 12.00 is greater than the scrolling resolution
						//therefore clear all the flags
	                    TwelveOclockPointFound=0;
	                    RelativeDisplacement=TwelveOclockDisplacement;
	                    TwelveOclockDisplacement=0;
					}
				}
	            else
				{
					//Relative displacement anywhere else on the disc.
					if (FirstTimeTouchWheelTouched==2)
					{
						tempResult = AbsPosOnTw;
						tempResult = tempResult - PositionOnFirstTouch;
    	                RelativeDisplacement = tempResult;
					}
				}
				
				//Select different items in the listbox
				if (abs(RelativeDisplacement) >= DisplayItemsResolution)
				{
					PositionOnFirstTouch = AbsPosOnTw;
					if (RelativeDisplacement > 0)
					{
						if(MovementDetected==1)
						{
							if (UpdateListBoxDownOnce==1)
							{	//Go down
								UpdateListBoxDownOnce=0;
								UpdateListBoxUpOnce=1;
								ScrollwheelStatus &= 0xCFFF;
								ScrollwheelStatus |= 0x1000;
							}
						}
						else
						{	//Stay static
							ScrollwheelStatus &= 0xCFFF;
						}
					}
					else if (RelativeDisplacement < 0)
					{
						if(MovementDetected==1)
						{
							if (UpdateListBoxUpOnce==1)
							{	//Go up
								UpdateListBoxUpOnce=0;
								UpdateListBoxDownOnce=1;
								ScrollwheelStatus &= 0xCFFF;
								ScrollwheelStatus |= 0x2000;
							}
						}
						else
						{	//Stay static
							ScrollwheelStatus &= 0xCFFF;
						}
					}
					MovementDetected = 1;
				}
				else
				{	
					//Stay static
					UpdateListBoxDownOnce=1;
					UpdateListBoxUpOnce=1;
					ScrollwheelStatus &= 0xCFFF;
				}
			}
			else
			{	
				//Stay static
				UpdateListBoxDownOnce=1;
				UpdateListBoxUpOnce=1;
				ScrollwheelStatus &= 0xCFFF;
			}
		}//End if((TouchError & 0x01)!=0x01)
				
		
		//= Clear the tap if we're scrolling =
		//====================================
		//We have a new position, if it is different from the previous one and we are still within the
		//the tapping time, then we check the positions. If we are moving, then we clear the tapping bit.
		if( ((TouchError& 0x1)!=0x1) && ScrollWheelTouchDownCounter<T_MAX_TOUCHING)
		{
			if(MovementDetected==1)
			{
				ScrollWheelTouchDownCounter=T_MAX_TOUCHING+1;
				TappingFlag=0; //Clear tapping bit on slider
				EnableTapDisplayOnTw=0;	//We're moving, so don't display the tap;
				//MultiTapDetected=0;
			}
			else
				EnableTapDisplayOnTw=1;	//We haven't moved by far enough, so a tap is still valid
		}
	} //End if(ScrollwheelFlag==1)

	if (FastScrollDetected==1 && TouchwheelFlag==0)
	{
		if (NumberOfUpdates>0)
		{
			if (FastScrollUpdateCounter>0)
			{
				FastScrollUpdateCounter--;
				ScrollwheelStatus &= 0xCFFF;
			}
			else
			{
				NumberOfUpdates--;
				FastScrollUpdateCounter=2;
				if (FastScrollDirection==UP)
				{
					//Go up
					ScrollwheelStatus &= 0xCFFF;
					ScrollwheelStatus |= 0x2000;
				}
				else if (FastScrollDirection==DOWN)
				{
					//Go down
					ScrollwheelStatus &= 0xCFFF;
					ScrollwheelStatus |= 0x1000;
				}
			}
		}
		else
		{
			FastScrollDetected=0;
			ScrollwheelStatus &= 0xCFFF;
		}
	}
	//= Format position data =
	//========================
	ScrollwheelStatus &= 0x33FF;
	ScrollwheelStatus |= ((TouchError <<2)<<8);
	
	if (TouchwheelFlag==1)
	{
		//Compute position registers
		if ((TouchError & 0x1)!=0x1)
		{
			ScrollwheelStatus |= 0x8000;
			ScrollwheelStatus &= 0xFF00;
			ScrollwheelStatus |= AvgPositionOnTw;
		}
	}
	else	//Not touching...
	{
		//Clear finger valid bit and go up go down commands
		ScrollwheelStatus &= 0x0FFF;
		if(TappingFlag==1)
			ScrollwheelStatus |= 0x4000;
	}
	return (ScrollwheelStatus);
}



//---------------------------------
//FindHighestAndLowestStagesUsed()
//---------------------------------
//Function that finds which is the highest and which is the lowest sensor touched during a scroll.
//If there is a big finger touching the sensor, then the BigFingerTouching flag is set. if there 
//is more than one finger touching, then TwoFingerTouching flag is set. If any of these 2 flags
//are set, the function returns an error otherwise 0 for normal operation.
static BYTE FindHighestAndLowestStagesUsed(const WORD InterruptStatusRegister, const WORD LowestStageOfTheSensor, const WORD HighestStageOfTheSensor, const BYTE NumberOfStagesUsed, const BigFingerLevel, BYTE *LowestSensorTouched, BYTE *HighestSensorTouched)
{
	WORD xdata ShiftValue;
	BYTE xdata i, xdata InterruptBitCounter;
	bit LowestSensorTouchedFound, HighestSensorTouchedFound;
	bit TwoFingerTouching, BigFingerTouching;
	
	//Initialisation
	LowestSensorTouchedFound=0;
	HighestSensorTouchedFound=0;	
	*LowestSensorTouched=0xFF;
	*HighestSensorTouched=0xFF;
	InterruptBitCounter=0;
	TwoFingerTouching=0;
	BigFingerTouching=0;
	
	if(((InterruptStatusRegister & LowestStageOfTheSensor) == LowestStageOfTheSensor) && ((InterruptStatusRegister & HighestStageOfTheSensor) == HighestStageOfTheSensor))
	{
		//Touchwheel specific 
		//===================
		//Comment out the whole if statement above for a slider or any others.
		
		//Start from the highest stage and walk back down to the lowest stage. The value of the
		//lowest sensor touched will be bigger than the value of the highest sensor touched.
		ShiftValue=HighestStageOfTheSensor;
		for(i=NumberOfStagesUsed;i!=0;i--)
		{
			if(((InterruptStatusRegister & ShiftValue) == 0) && LowestSensorTouchedFound==0)
			{
				//Lowest sensor touched is found here
				*LowestSensorTouched=i;
				LowestSensorTouchedFound=1;								
			}
			else if((InterruptStatusRegister & ShiftValue) == ShiftValue) 
			{
				InterruptBitCounter++;
				if (LowestSensorTouchedFound==1 && HighestSensorTouchedFound==0)
				{
					//Highest sensor touched is found here
					*HighestSensorTouched=i-1;
					HighestSensorTouchedFound=1;
				}
			}
			//Code for detecting if there are 2 fingers on the entire sensor
			if (LowestSensorTouchedFound==1 && HighestSensorTouchedFound==1 && ((InterruptStatusRegister & ShiftValue) == 0) && i<(*LowestSensorTouched))
				TwoFingerTouching=1;
			
			ShiftValue=ShiftValue>>1;
		}
	}
	else
	{	
		//Code common to all sensors
		//==========================
		ShiftValue=LowestStageOfTheSensor;
		for(i=0;i<NumberOfStagesUsed;i++)
		{
			//Found the lowest sensor touched
			if((InterruptStatusRegister & ShiftValue) == ShiftValue) 
			{
				if(LowestSensorTouchedFound==0)
				{
					//Lowest sensor touched is found here
					*LowestSensorTouched=i;
					LowestSensorTouchedFound=1;
				}
				//If last sensor is set, then it is also the highest sensor touched
				if(i==(NumberOfStagesUsed-1) && LowestSensorTouchedFound==1 && HighestSensorTouchedFound==0)
				{					
					*HighestSensorTouched=i;
					HighestSensorTouchedFound=1;
				}				
			}
			//Found the highest sensor touched
			else if(((InterruptStatusRegister & ShiftValue) == 0) && LowestSensorTouchedFound==1 && HighestSensorTouchedFound==0)
			{
				//Highest sensor touched is found here
				*HighestSensorTouched=i-1;
				HighestSensorTouchedFound=1;
			}			
			ShiftValue=ShiftValue<<1;
		}
		
		//Code for detecting if there are 2 fingers on the entire sensor and if there is a big 
		//finger touching it. We count the number of interrupts set and if it is greater than 
		//"BigFingerLevel", then we set the "BigFingerTouching" flag.
		ShiftValue=LowestStageOfTheSensor;		
		for(i=0;i<NumberOfStagesUsed;i++)
		{
			if((InterruptStatusRegister & ShiftValue)==ShiftValue)
			{
				InterruptBitCounter++;
				if((i>(*HighestSensorTouched)) || (i<(*LowestSensorTouched)))
					TwoFingerTouching=1;
			}
			ShiftValue=ShiftValue<<1;
		}
	}
	if(InterruptBitCounter>=BigFingerLevel)
		BigFingerTouching=1;
	//Return error code
	return (((BYTE)TwoFingerTouching) | (((BYTE)BigFingerTouching)<<1));
}


//---------------------------------
//InitialiseTouchWheel()
//---------------------------------
//Function that initialise the touchwheel. It is called on power up and when calibrating all the sensors
void InitialiseTouchWheel(void)
{
	WORD xdata AmbientValAddress;
	BYTE i;

	AmbientValAddress = STAGE0_AMBIENT;
	
	// Initialise the scaled down value of the distance between the current value - ambient value.
	for(i=0;i<NB_OF_SENSORS_FOR_TW;i++)
	{				
		//Read ambient values
		ReadFromAD7147(AmbientValAddress, 1, AmbientValues, i);
		SensorValues[i]=0;
		//Update stage variables for the next loop
		AmbientValAddress=AmbientValAddress+NB_OF_REGS_PER_STAGE;
	}
	
	NoTouchCounterOnTw = 0;
	DisplayItemsResolution=(NUMBER_OF_POSITIONS_WANTED+1)/DISPLAY_ITEMS_CONSTANT;
	ScrollwheelStatus = 0;
	PositionRatio = INITIAL_POSITION_RATIO;
	PositionOffset = INITIAL_OFFSET_POSITION;
}


DWORD ApplyWeight(const WORD SensorVal, const BYTE Order)
{
	DWORD temp;

	temp = SensorVal;
	temp *= WEIGHT_FACTOR;
	temp *= Order;
	return (temp);
}
