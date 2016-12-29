#include "Include\General Definitions.h"
#include "Include\AD7147RegMap.h"


//---------------------------------
//Global variables
//---------------------------------
WORD xdata AD7147Registers[NUMBER_OF_AD7147_REGISTERS];
								//[0...23 inc]=1st set of registers [0...23 inc]
								//Element[23] = 0x17 = ID register @ 0x17
								
//---------------------------------
//Function prototypes
//---------------------------------

//Local functions
//---------------------
void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);
void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);

//External functions
//---------------------
extern void WriteToAD7147ViaSpi(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
extern void ReadFromAD7147ViaSpi(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);


//---------------------------------
//Function definitions
//---------------------------------

//---------------------------------
//WriteToAD7147();
//---------------------------------
//This function writes to the AD7147 either via I2C.
//--------------------------------------------------------------------------------
void WriteToAD7147(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer)
{
	WriteToAD7147ViaSpi(RegisterAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}


//---------------------------------
//ReadFromAD7147();
//---------------------------------
//This function reads from the AD7147 via I2C.
//--------------------------------------------------------------------------------
void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	ReadFromAD7147ViaSpi(RegisterStartAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}