//---------------------------------
//SPI/I2C definitions
//---------------------------------
sbit P3_4=0xB4; // CS
sbit P3_5=0xB5; // CLK 
sbit P3_6=0xB6; // DOUT
sbit P3_7=0xB7; // DIN
sbit P2_7=0xA7;	// GPIO
sbit P3_2=0xB2; // To make sure that the INT0 pin on the ADuC841 will be an input.

#define	DOUT			P3_7    // DOUT on the AD7147
#define	DIN				P3_6    // DIN on the AD7147
#define CLK				P3_5
#define CS				P3_4	// CS on the AD7147
#define GPIO			P2_7
#define ADUC841_INT0	P3_2

#define SPICLKPW		1

//---------------------------------
//General definitions
//---------------------------------
#define	BYTE	unsigned char
#define WORD	unsigned int
#define DWORD	unsigned long int

//For the memory map of the ADuC841 registers
#define STAGE_START_ADDRESS		10
#define NB_OF_REGS_PER_STAGE	36
#define NB_OF_CONN_REGS			8
#define REGISTER_LENGTH			16


//---------------------------------
//Global variables
//---------------------------------
WORD xdata AD7147Registers[24];	//[0...23 inc]=1st set of registers [0...23 inc]
								//Element[23] = 0x17 = ID register @ 0x17

//---------------------------------
//Function prototypes
//---------------------------------
void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
void WriteToAD7147ViaSpi(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
void ReadFromAD7147ViaSpi(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);


//---------------------------------
//Function definitions
//---------------------------------


//---------------------------------
//WriteToAD7147();
//---------------------------------
//This function writes to the AD7147 either via SPI.
//--------------------------------------------------------------------------------
void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer)
{
	WriteToAD7147ViaSpi(RegisterAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}

//---------------------------------
//ReadFromAD7147();
//---------------------------------
//This function reads from the AD7147 either via SPI.
//--------------------------------------------------------------------------------
void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	ReadFromAD7147ViaSpi(RegisterStartAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}
