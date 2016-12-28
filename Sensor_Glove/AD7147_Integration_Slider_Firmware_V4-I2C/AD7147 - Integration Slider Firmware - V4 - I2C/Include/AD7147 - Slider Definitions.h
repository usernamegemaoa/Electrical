//Slider definitions
//------------------

//"NB_OF_SENSORS_FOR_SLIDER"
//Slider is connected from Stage 0 to Stage 5
#define NB_OF_SENSORS_FOR_SLIDER					8

//"STAGES_USED_FOR_SLIDER"
//This constant represents all the possible stages
//that can be activated touching the slider
#define STAGES_USED_FOR_SLIDER						0x00FF

#define LOWER_SENSOR_STAGE							0x0001
#define NUMBER_OF_SENSORS_ON_FOR_BIG_FINGER			4

//Definitions for the tap
#define T_MIN_TOUCHING								2	//2*9ms = 18ms
#define T_MAX_TOUCHING								40	//40*9ms = 360ms
#define T_MIN_NO_TOUCHING							20 	//20*9ms = 180ms
#define DISPLAY_AFTER_A_TAP							12	//12*9ms = 108ms

//"MAX_TOUCHDOWN_COUNTER_TIME"
//We stop counting after this amount of Time touching
// on the Slider or Buttons
//255*9ms = 3seconds
#define MAX_TOUCHDOWN_COUNTER_TIME					400 

//"MAX_NO_TOUCH_COUNTER"
//This definition is for stopping our NoTouchCounterOnSlider after a certain number of interrupts
//This should be changed depending on Interrupt Timeout. This is based on 9ms.
 #define MAX_NO_TOUCH_COUNTER						104	//104*9ms = 936ms


//"MIN_NUMBER_OF_UPDATES"
//This definition is used to set the mimimum number of updates 
//when a quick/small swipe is registered
#define MIN_NUMBER_OF_UPDATES						9


//"DISPLAY_ITEMS_CONSTANT"
//DISPLAY_ITEMS_CONSTANT determines how many items 
//can be selected in one scroll
#define DISPLAY_ITEMS_CONSTANT						12

//"LISTBOX_SLOW_UPDATE"
//LISTBOX_SLOW_UPDATE determines the number of interrupts 
//required before starting to autoscroll
#define LISTBOX_SLOW_UPDATE							16

//"LISTBOX_QUICK_UPDATE" determines the number of interrupts 
//required before the next update when autoscrolling
#define LISTBOX_QUICK_UPDATE						4

//"Up/Down" definitions
#define UP											1
#define DOWN										0

//"DISPLACEMENT_OFFSET"
//This is applied to the Unfilterd Slider response to 
//bring smallest value to zero.
#define DISPLACEMENT_OFFSET							3

//"NUMBER_OF_POSITIONS"
//Maximal position available on the Slider.
#define NUMBER_OF_POSITIONS							127

//"PIXEL_RESOLUTION"
#define PIXEL_RESOLUTION							19.5