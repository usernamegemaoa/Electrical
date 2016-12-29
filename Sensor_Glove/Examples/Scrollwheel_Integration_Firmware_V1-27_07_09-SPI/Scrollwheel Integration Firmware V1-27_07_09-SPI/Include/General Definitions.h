//---------------------------------
//SPI definitions
//---------------------------------
sbit P3_4=0xB4; // CS
sbit P3_5=0xB5; // CLK 
sbit P3_6=0xB6; // DOUT
sbit P3_7=0xB7; // DIN
sbit P2_7=0xA7;	// GPIO
sbit P3_2=0xB2; // To make sure that the INT0 pin on the ADuC841 will be an input.

#define	DOUT			P3_6    // DOUT in SPI mode
#define	DIN				P3_7    // DIN in SPI mode
#define CLK				P3_5
#define CS				P3_4	// CS in SPI mode
#define GPIO			P2_7
#define ADUC841_INT0	P3_2

//---------------------------------
//General definitions
//---------------------------------
typedef	unsigned char		BYTE;
typedef	unsigned int		WORD;
typedef	unsigned long int	DWORD;

//For the initialisation after power up
#define NB_OF_INT								3
#define NUMBER_OF_INTS_BEFORE_THRES_INT_MODE	13

#define REGISTER_LENGTH			16

#define NB_OF_REGS_PER_STAGE	36

#define NUMBER_OF_AD7147_REGISTERS			24	//[0...23 inc]=1st set of registers [0...23 inc]
												//Element[23] = 0x17 = ID register @ 0x17