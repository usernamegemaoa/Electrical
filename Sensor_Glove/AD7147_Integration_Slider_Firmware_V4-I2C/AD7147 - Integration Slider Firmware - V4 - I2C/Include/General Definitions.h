sbit P3_4=0xB4; // ADD1
sbit P3_5=0xB5; // CLK 
sbit P3_6=0xB6; // SDA
sbit P3_7=0xB7; // ADD0
sbit P2_7=0xA7;	// GPIO
sbit P3_2=0xB2; // To make sure that the INT0 pin on the ADuC841 will be an input.

#define	SDA				P3_6  
#define	ADD0			P3_7  
#define CLK				P3_5
#define ADD1			P3_4
#define GPIO			P2_7
#define ADUC841_INT0	P3_2

#define AD7147_ADD		0x2C

//---------------------------------
//General definitions
//---------------------------------
#define	BYTE	unsigned char
#define WORD	unsigned int
#define DWORD	unsigned long int

#define	ADUC841			0
#define	AD7147			1

#define REGISTER_LENGTH			16

//For the initialisation after power up
#define NB_OF_INT				3

//For the memory map of the ADuC841 registers
#define STAGE_START_ADDRESS		10
#define NB_OF_REGS_PER_STAGE	36
#define NB_OF_CONN_REGS			8

#define NUMBER_OF_AD7147_REGISTERS				23	//[0...23 inc]=1st set of registers [0...23 inc]

//---------------------------------
//Definitions
//---------------------------------
//POWER_UP_INTERRUPT: Stage 0 to 11 only. A low limit interrupt on these bits will force calibration
#define POWER_UP_INTERRUPT							0x0CFF
#define STAGE_HIGH_INT_EN_VALUE						0x0CFF
#define NUMBER_OF_INTS_BEFORE_THRES_INT_MODE		18
