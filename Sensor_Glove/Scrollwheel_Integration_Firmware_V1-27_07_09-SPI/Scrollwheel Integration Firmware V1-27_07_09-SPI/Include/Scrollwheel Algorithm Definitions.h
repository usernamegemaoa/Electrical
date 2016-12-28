//AD7147 Configuration Definitions
//================================
//Stage 0, 1, 2, 3, 7, 8 and 9. Any low limit interrupt on these bits will
//force calibration
#define POWER_UP_INTERRUPT						 	0x01FF
#define STAGE_HIGH_INT_EN_VALUE						0x01FF


//Scrollwheel definitions
//=======================

//"NB_OF_SENSORS_FOR_TW"
//This definition corresponds to the number of stages used
//for the scrollwheel.
#define NB_OF_SENSORS_FOR_TW						8


//"NUMBER_OF_SENSORS_ON_FOR_BIG_FINGER"
//This constant defines how many threshold interrupts need to be 
//set to register a big finger on the scrollwheel.
#define NUMBER_OF_SENSORS_ON_FOR_BIG_FINGER			3

//"LOWER_SENSOR_STAGE"
//This constant defines the interrupt threshold status of the bit 
//used in the lower stage of the scrollwheel. Here Stage 6.
#define LOWER_SENSOR_STAGE							0x0001

//"HIGHER_SENSOR_STAGE"
//This constant defines the interrupt threshold status of the bit 
//used in the higher stage of the scrollwheel. Here Stage 11.
#define HIGHER_SENSOR_STAGE							0x0080	

//"NUMBER_OF_STAGES_BEING_USED"
//This constant defines the all the interrupt threshold status bits
//used in the scrollwheel.
#define NUMBER_OF_STAGES_BEING_USED					0x00FF

//"NUMBER_OF_POSITIONS"
//Maximal position available.
#define NUMBER_OF_POSITIONS_WANTED					127

//"NUMBER_OF_POSITIONS_TO_ACTIVATE_LISTBOX"
//This constant defines how many positions need to be 
//scrolled through before updating the listbox
#define NUMBER_OF_POSITIONS_TO_ACTIVATE_LISTBOX		8

//"DISPLAY_ITEMS_CONSTANT"
//This constant determines how many items can be selected 
//in one full turn
#define DISPLAY_ITEMS_CONSTANT						16

//Definitions for the tap
#define T_MIN_TOUCHING								1
#define T_MAX_TOUCHING								8
#define T_MAX_NO_TOUCHING							5
#define DISPLAY_AFTER_A_TAP							3

//"UP/DOWN"
//Defines the sense of scrolling
#define UP											2
#define DOWN										1

//#define LISTBOX_SLOW_UPDATE							20
#define LISTBOX_QUICK_UPDATE						6		

#define NB_OF_VALUES_IN_CALCULATIONS				5

//"WEIGHT_FACTOR"
//The WEIGHT_FACTOR is a constant used in the formula to 
//detemine the mean value. The bigger this value, the bigger
//the number of positions. If this value is too big, then
//the linearity gets worse.
#define WEIGHT_FACTOR								30

//"INITIAL_OFFSET_POSITION"
//This constant is subtracted to the "MeanValue" variable
#define INITIAL_OFFSET_POSITION						80

//"OFFSET_POSITION_CLAMP"
//This constant prevents the "PositionOffset" from reaching a big value
#define OFFSET_POSITION_CLAMP						120

//"INITIAL_POSITION_RATIO"
//This definition is used to rescale the "MeanValue" variable with
//the "INITIAL_OFFSET_POSITION" subtracted from it. 
//"INITIAL_POSITION_RATIO" rescales to NUMBER_OF_POSITIONS_WANTED"
#define INITIAL_POSITION_RATIO						175
