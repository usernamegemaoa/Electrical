#include <math.h>
#include "Include\General Definitions.h"
#include "Include\AD7147RegMap.h"
#include "Include\AD7147 - Slider Definitions.h"


//---------------------------------
//Function prototypes
//---------------------------------
//External functions
extern void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
extern void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);

//Local functions
void ConfigAD7147(void);
void InitialiseSlider(void);
static BYTE FindHighestAndLowestStagesUsed(const WORD InterruptStatusRegister, const WORD LowestStageOfTheSensor, const BYTE NumberOfStagesUsed, const BigFingerLevel);

//---------------------------------
//Global variables
//---------------------------------

//External global variables 
//-------------------------
extern WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];
extern BYTE xdata InterruptCounterForThresIntMode;

//Local Global variables
//----------------------
//SLIDER
WORD xdata SliderStatus;
WORD xdata AmbientValues[NB_OF_SENSORS_FOR_SLIDER];
WORD xdata SensorValues[NB_OF_SENSORS_FOR_SLIDER];

//Variables for activation
BYTE xdata FirstTimeSliderTouched;
WORD xdata SliderTouchDownCounter;
bit SliderFlag;

//Variables for tapping on the Slider
BYTE xdata ReturnTappingCounterOnSlider;
BYTE xdata NoTouchCounterOnSlider;
WORD xdata ListBoxUpdateCounterValue;
WORD xdata ListBoxUpdateCounter;
bit EnableTapDisplayOnSlider;
bit TappingFlag;

BYTE xdata NumberOfUpdates;
BYTE xdata SensorWithHighestValue;

//Position variables	
WORD xdata SliderPosition;
WORD xdata AveragePosition;

//Relative position variables
bit MovedSinceActivation;
bit PositionOnFirstTouchRecorded;
bit FastScrollDirection;
bit RecordSliderCounterOnce;
bit FastScrollDetected;
BYTE xdata MinimalNumberOfInterruptsAfterLiftingOff;
BYTE xdata PositionOnFirstTouch;
BYTE xdata FastScrollUpdateCounter;
WORD xdata DisplayItemsResolution;
WORD xdata PositionOnActivation;

//----------------------
//Function declarations
//----------------------
WORD GetNewSliderUpdate(void);

//---------------------
//Function definitions
//---------------------
WORD GetNewSliderUpdate(void)
{
	BYTE i;
	WORD xdata AmbientValueAddress;
	BYTE xdata TouchError;	
	WORD xdata TempAveragePosition;
	DWORD xdata TempSliderPosition;
	DWORD xdata A_parameter;
	DWORD xdata B_parameter;
	
	
	if((AD7147Registers[STAGE_HIGH_LIMIT_INT] & STAGES_USED_FOR_SLIDER) != 0x0000)
	{	
	
		//If any slider stages is activated then we read data from the AD7147
		ReadFromAD7147(ADCRESULT_S0, NB_OF_SENSORS_FOR_SLIDER, AD7147Registers, ADCRESULT_S0);		
		
		AmbientValueAddress=STAGE0_AMBIENT;
		for(i=0;i<NB_OF_SENSORS_FOR_SLIDER;i++)
		{
			ReadFromAD7147(AmbientValueAddress, 1, AmbientValues, i);
			AmbientValueAddress=AmbientValueAddress+NB_OF_REGS_PER_STAGE;
		}
	}			
	//Calculate the sensor responses
	for(i=0;i<NB_OF_SENSORS_FOR_SLIDER;i++)
	{			
		if(AD7147Registers[ADCRESULT_S0+i]>AmbientValues[i]) 
			SensorValues[i]=abs(AD7147Registers[ADCRESULT_S0+i]-AmbientValues[i]);
		else
			SensorValues[i]=0;
	}	

	//Check for touch errors	
	TouchError = FindHighestAndLowestStagesUsed(AD7147Registers[STAGE_HIGH_LIMIT_INT], LOWER_SENSOR_STAGE, 
									  		    NB_OF_SENSORS_FOR_SLIDER, NUMBER_OF_SENSORS_ON_FOR_BIG_FINGER);
	
	//= Slider activation
	//===================
	if(((AD7147Registers[STAGE_HIGH_LIMIT_INT] & STAGES_USED_FOR_SLIDER) != 0x0000))
	{
		TappingFlag=0;//Touching therefore clear the tapping flag.
		RecordSliderCounterOnce = 1;
		SliderStatus &= 0xBFFF;
		//On Touch
		FastScrollDetected=0;				
		
		//Check if the user left enough time between 2 taps.
		if (NoTouchCounterOnSlider > T_MIN_NO_TOUCHING)
			ReturnTappingCounterOnSlider=DISPLAY_AFTER_A_TAP; //Reset counter for displaying the tap
		else
			ReturnTappingCounterOnSlider=0;
		
		if (SliderTouchDownCounter<100)
		{			
			//Touching for more than 0.5sec, hence clear the NoTouchCounter
			if (SliderTouchDownCounter > (T_MIN_TOUCHING+T_MAX_TOUCHING))
				NoTouchCounterOnSlider=0;
			
			SliderTouchDownCounter++;
		}
		else
		{
			NoTouchCounterOnSlider=0;
			SliderTouchDownCounter=MAX_TOUCHDOWN_COUNTER_TIME;
		}
		
		if (SliderTouchDownCounter>(T_MIN_TOUCHING))
			SliderFlag=1;
		
	}
	else if((AD7147Registers[STAGE_HIGH_LIMIT_INT] & STAGES_USED_FOR_SLIDER) == 0x0000)
	{	
		//Listbox Variables
		ListBoxUpdateCounterValue=LISTBOX_SLOW_UPDATE;
		ListBoxUpdateCounter=0;
		PositionOnFirstTouchRecorded=0;
		
		//Slider Activation Variables
		SliderFlag=0;
		FirstTimeSliderTouched=0;		
		SliderStatus &= 0x7FFF;
		
		//Keep scrolling through list box if we were moving fast on the slider
		if (RecordSliderCounterOnce==1)
		{
			RecordSliderCounterOnce=0;
			//Fast scroll detection on Y axis
			//===============================
			if (SliderTouchDownCounter > T_MIN_TOUCHING && SliderTouchDownCounter < T_MAX_TOUCHING)
			{
				NumberOfUpdates = abs(PositionOnActivation - SliderPosition) / DisplayItemsResolution;

				if (abs(PositionOnActivation - SliderPosition) > (NUMBER_OF_POSITIONS>>4))
					NumberOfUpdates = NumberOfUpdates + 5;

				if (NumberOfUpdates>1 && NumberOfUpdates<9)
					NumberOfUpdates = MIN_NUMBER_OF_UPDATES;
				else if (NumberOfUpdates==1)
					NumberOfUpdates = 0;
				
				if (SliderPosition > PositionOnActivation)
					FastScrollDirection = DOWN;
				else if (SliderPosition < PositionOnActivation)
					FastScrollDirection = UP;
				FastScrollUpdateCounter=LISTBOX_QUICK_UPDATE;
				
				//Check if there will be enough interrupts after lifting off before to switch to threshold mode.
				MinimalNumberOfInterruptsAfterLiftingOff = NumberOfUpdates * LISTBOX_QUICK_UPDATE;
				if ((MinimalNumberOfInterruptsAfterLiftingOff) > NUMBER_OF_INTS_BEFORE_THRES_INT_MODE)
					InterruptCounterForThresIntMode = MinimalNumberOfInterruptsAfterLiftingOff + 10;
								
				FastScrollDetected=1;
				
				if (MovedSinceActivation==0)
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
		
		MovedSinceActivation=0;

		//= Work out the tap
		//==================
		if (NoTouchCounterOnSlider<MAX_NO_TOUCH_COUNTER)
			NoTouchCounterOnSlider++;
		
		if (SliderTouchDownCounter>T_MIN_TOUCHING && SliderTouchDownCounter<T_MAX_TOUCHING &&
			NoTouchCounterOnSlider > SliderTouchDownCounter)
		{				
			//Set a bit for a certain number of interrupts so that the user applications sees it.
			//This is done with a decounter that is reloaded when we are touching
			if (ReturnTappingCounterOnSlider>0 && EnableTapDisplayOnSlider==1)
			{
				ReturnTappingCounterOnSlider--;
				TappingFlag=1;	//Tap detected here
			}
			else
			{
				SliderTouchDownCounter=0;
				TappingFlag=0;
			}
		}
		else
		{
			if (SliderTouchDownCounter>T_MAX_TOUCHING)
				SliderTouchDownCounter=0;
			
			ReturnTappingCounterOnSlider=0;
			TappingFlag=0;//Cleared tapping bit after lift off 
		}
	}	
		
	if(SliderFlag==1)
	{
		//===========================================================
		//============== ABSOLUTE POSITION CALCULATION ==============
		//===========================================================
		if((TouchError & 0x01)!=0x01)
		{
			//===========================================================
			//Find peak among the response of the measurements on Slider
			//===========================================================
			SensorWithHighestValue=0;
			for (i=0;i<NB_OF_SENSORS_FOR_SLIDER;i++)
			{
				if (i<(NB_OF_SENSORS_FOR_SLIDER-1))
				{
					if ((SensorValues[i] > SensorValues[i+1]) && 
						(SensorValues[i] > SensorValues[SensorWithHighestValue]))
						SensorWithHighestValue=i;
				}
				else if (i==(NB_OF_SENSORS_FOR_SLIDER-1))
				{
					if ((SensorValues[i] > SensorValues[i-1]) && 
						(SensorValues[i] > SensorValues[SensorWithHighestValue]))
						SensorWithHighestValue=i;
				}
			}

			//==================================================================
			//============== SLIDER ABSOLUTE POSITION CALCULATION ==============
			//==================================================================
			switch (SensorWithHighestValue)
			{
				case 0:
					//Calculate "A" parameter
					A_parameter = SensorValues[1];
					//Calculate "B" parameter
					B_parameter = SensorValues[0] + SensorValues[1];					
										
					TempSliderPosition = (PIXEL_RESOLUTION * A_parameter);
					TempSliderPosition = TempSliderPosition / B_parameter;
					
					if(TempSliderPosition>=DISPLACEMENT_OFFSET)
						SliderPosition=(WORD)(TempSliderPosition-DISPLACEMENT_OFFSET);
					else
						SliderPosition=(WORD)TempSliderPosition;						
					break;

				case (NB_OF_SENSORS_FOR_SLIDER-1):
					//Calculate "A" parameter
					A_parameter = SensorValues[NB_OF_SENSORS_FOR_SLIDER-1] * (NB_OF_SENSORS_FOR_SLIDER-1);
					A_parameter = A_parameter + SensorValues[NB_OF_SENSORS_FOR_SLIDER-2] * (NB_OF_SENSORS_FOR_SLIDER-2);
					//Calculate "B" parameter
					B_parameter = SensorValues[NB_OF_SENSORS_FOR_SLIDER-1] + SensorValues[NB_OF_SENSORS_FOR_SLIDER-2];// + SensorValues[NB_OF_SENSORS_FOR_SLIDER-3];
										
					TempSliderPosition = (PIXEL_RESOLUTION * A_parameter);
					TempSliderPosition = TempSliderPosition / B_parameter;					
					
					if(TempSliderPosition>=DISPLACEMENT_OFFSET)
						SliderPosition=(WORD)(TempSliderPosition-DISPLACEMENT_OFFSET);
					else
						SliderPosition=(WORD)TempSliderPosition;
					break;
				default:
					//Calculate "A" parameter
					A_parameter = SensorValues[SensorWithHighestValue-1] * (SensorWithHighestValue-1);
					A_parameter = A_parameter + SensorValues[SensorWithHighestValue] * (SensorWithHighestValue);
					A_parameter = A_parameter + SensorValues[SensorWithHighestValue+1] * (SensorWithHighestValue+1);
					
					//Calculate "B" parameter
					B_parameter = SensorValues[SensorWithHighestValue-1] + SensorValues[SensorWithHighestValue] + SensorValues[SensorWithHighestValue+1];
					
					TempSliderPosition = (PIXEL_RESOLUTION * A_parameter);
					TempSliderPosition = TempSliderPosition / B_parameter;
					
					if(TempSliderPosition>=DISPLACEMENT_OFFSET)
						SliderPosition=(WORD)(TempSliderPosition-DISPLACEMENT_OFFSET);
					else
						SliderPosition=(WORD)TempSliderPosition;
					break;
			}
			
			if(FirstTimeSliderTouched==0)
			{
				FirstTimeSliderTouched=1;
				if ((SliderPosition > (NUMBER_OF_POSITIONS-5)) || (SliderPosition < 5))
					ListBoxUpdateCounterValue=LISTBOX_SLOW_UPDATE;
				else
					ListBoxUpdateCounterValue = LISTBOX_QUICK_UPDATE;
			}
			
			//= Initialise some of the position variables on activation
			//=========================================================
			if (PositionOnFirstTouchRecorded==0)
			{
				PositionOnFirstTouchRecorded=1;
				PositionOnFirstTouch = SliderPosition;
				PositionOnActivation = SliderPosition;
				AveragePosition = SliderPosition;
			}
			else if (PositionOnFirstTouchRecorded==1)
			{
				//= Filter out slider response
				//============================
				TempAveragePosition = AveragePosition * 6 + SliderPosition * 4.0;
				AveragePosition = TempAveragePosition / 10;
				
				if (AveragePosition > NUMBER_OF_POSITIONS)
					AveragePosition = NUMBER_OF_POSITIONS;
				
				//= Code for relative positions for the slider
				//============================================
				//Move through items in the listbox as long as no buttons are being set
				if (SliderPosition < 5)
				{	//Auto scroll up
					ListBoxUpdateCounter++;
					if (ListBoxUpdateCounter == ListBoxUpdateCounterValue)
					{
						ListBoxUpdateCounter=0;
						ListBoxUpdateCounterValue=LISTBOX_QUICK_UPDATE;
						//Go up
						SliderStatus &= 0xCFFF;
						SliderStatus |= 0x2000;
					}
					else
					{
						SliderStatus &= 0xCFFF;
					}
				}				
				else if(SliderPosition > (NUMBER_OF_POSITIONS-5) )
				{	//Auto scroll down
					ListBoxUpdateCounter++;
					if (ListBoxUpdateCounter == ListBoxUpdateCounterValue)
					{
						ListBoxUpdateCounter=0;
						ListBoxUpdateCounterValue=LISTBOX_QUICK_UPDATE;
						//Go down
						SliderStatus &= 0xCFFF;
						SliderStatus |= 0x1000;
					}
					else
					{
						SliderStatus &= 0xCFFF;
					}
				}
				else
				{
					if (abs(SliderPosition - PositionOnFirstTouch) > DisplayItemsResolution)
					{
						//Create list box commands for the Y axis
						if (SliderPosition > PositionOnFirstTouch)
						{	//Go down
							SliderStatus &= 0xCFFF;
							SliderStatus |= 0x1000;
						}
						else if (SliderPosition < PositionOnFirstTouch)
						{	//Go up
							SliderStatus &= 0xCFFF;
							SliderStatus |= 0x2000;
						}
						PositionOnFirstTouch = SliderPosition;
						MovedSinceActivation=1;
					}
					else
					{
						SliderStatus &= 0xCFFF;
					}
				}
			}//End else if (PositionOnFirstTouchRecorded==1)
		}//End if ((TouchError & 0x01)!=0x01)
	}

	//= Clear tap if we're scrolling
	//==============================
	//We have a new position, if it is different from the previous one and we are still within the
	//the tapping time, then we check the positions. If we are moving, then we clear the tapping bit.
	if( ((TouchError& 0x1)!=0x1) && SliderTouchDownCounter<T_MAX_TOUCHING)
	{
		if(MovedSinceActivation==1)
		{
			TappingFlag=0; //Clear tapping bit on slider
			EnableTapDisplayOnSlider=0;	//We're moving, so don't display the tap;
		}
		else
			EnableTapDisplayOnSlider=1;	//We haven't moved by far enough, so a tap is still valid
	}
	
	//= Format the position data
	//==========================
	SliderStatus &= 0xF3FF;
	SliderStatus |= ((TouchError <<2)<<8);
	
	if (SliderFlag==1)
	{
		SliderStatus |= 0x8000;
		//Compute position registers
		if(((TouchError & 0x01)!=0x01))
		{
			SliderStatus &= 0xBC00;
			SliderStatus |= AveragePosition;
		}
	}
	else	//Not touching...
	{
		//Clear finger valid bit and go up go down commands
		SliderStatus &= 0x0FFF;
		//Return Tap
		if(TappingFlag==1)
			SliderStatus |= 0x4000;

		if (FastScrollDetected==1)
		{
			if (NumberOfUpdates>0)
			{
				if (FastScrollUpdateCounter>0)
				{
					SliderStatus &= 0xCFFF;
					FastScrollUpdateCounter--;
				}
				else
				{
					NumberOfUpdates--;
					FastScrollUpdateCounter=3;
					if (FastScrollDirection==UP)
					{
						//Go up
						SliderStatus &= 0xCFFF;
						SliderStatus |= 0x2000;
					}
					else if (FastScrollDirection==DOWN)
					{
						//Go down
						SliderStatus &= 0xCFFF;
						SliderStatus |= 0x1000;
					}
				}
			}
			else
			{
				FastScrollDetected=0;
				SliderStatus &= 0xCFFF;
			}
		}
	}
	return(SliderStatus);
}


//---------------------------------
//FindHighestAndLowestStagesUsed()
//---------------------------------
//Function that checks if there are 2 fingers touching the slider or if there is one big finger
static BYTE FindHighestAndLowestStagesUsed(const WORD InterruptStatusRegister, const WORD LowestStageOfTheSensor, const BYTE NumberOfStagesUsed, const BigFingerLevel)
{
	WORD xdata ShiftValue;
	BYTE xdata i, xdata InterruptBitCounter;
	BYTE xdata LowestSensorTouched=0xFF;
	BYTE xdata HighestSensorTouched=0xFF;
	bit LowestSensorTouchedFound, HighestSensorTouchedFound;
	bit TwoFingerTouching, BigFingerTouching;
	
	//Initialisation
	LowestSensorTouchedFound=0;
	HighestSensorTouchedFound=0;	
	
	InterruptBitCounter=0;
	TwoFingerTouching=0;
	BigFingerTouching=0;
	
	//For all sensors in use
	//======================
	ShiftValue=LowestStageOfTheSensor;
	for(i=0;i<NumberOfStagesUsed;i++)
	{
		//Found the lowest sensor touched
		if((InterruptStatusRegister & ShiftValue) == ShiftValue) 
		{
			if(LowestSensorTouchedFound==0)
			{
				//Lowest sensor touched is found here
				LowestSensorTouched=i;
				LowestSensorTouchedFound=1;
			}
			//If last sensor is set, then it is also the highest sensor touched
			if(i==(NumberOfStagesUsed-1) && LowestSensorTouchedFound==1 && HighestSensorTouchedFound==0)
			{					
				HighestSensorTouched=i;
				HighestSensorTouchedFound=1;
			}				
		}
		//Found the highest sensor touched
		else if(((InterruptStatusRegister & ShiftValue) == 0) && LowestSensorTouchedFound==1 && HighestSensorTouchedFound==0)
		{
			//Highest sensor touched is found here
			HighestSensorTouched=i-1;
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
			if((i>HighestSensorTouched) || (i<LowestSensorTouched))
				TwoFingerTouching=1;
		}
		ShiftValue=ShiftValue<<1;
	}

	if(InterruptBitCounter>=BigFingerLevel)
		BigFingerTouching=1;
	//Return error code
	return (((BYTE)TwoFingerTouching) | (((BYTE)BigFingerTouching)<<1));
}

//---------------------------------
//InitialiseSlider()
//---------------------------------
//Function that initialises the slider algorithm.
void InitialiseSlider(void)
{
	BYTE i;
	WORD xdata AmbientValueAddress;
		
	AmbientValueAddress = STAGE0_AMBIENT;
	for (i=0; i<NB_OF_SENSORS_FOR_SLIDER; i++)
	{	
		//Initialise ambient values
		ReadFromAD7147(AmbientValueAddress, 1, AmbientValues, i);
		AmbientValueAddress=AmbientValueAddress+NB_OF_REGS_PER_STAGE;
		SensorValues[i] = 0;
	}

	SliderStatus = 0x0000;
	ListBoxUpdateCounterValue = LISTBOX_SLOW_UPDATE;
	DisplayItemsResolution = NUMBER_OF_POSITIONS / DISPLAY_ITEMS_CONSTANT;	
	EnableTapDisplayOnSlider=1;
	NumberOfUpdates = 0;
}