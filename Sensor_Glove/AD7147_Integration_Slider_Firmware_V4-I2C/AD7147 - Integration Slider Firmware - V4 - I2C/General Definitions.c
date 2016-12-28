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
extern BYTE ReadFromAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToRead, WORD *DataBuffer, const WORD OffsetInBuffer);
extern BYTE WriteToAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToWrite, WORD *DataBuffer, const WORD OffsetInBuffer);



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
	WriteToAD7147ViaI2C(RegisterAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}


//---------------------------------
//ReadFromAD7147();
//---------------------------------
//This function reads from the AD7147 via I2C.
//--------------------------------------------------------------------------------
void ReadFromAD7147(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	ReadFromAD7147ViaI2C(RegisterStartAddress, NumberOfRegisters, DataBuffer, OffsetInBuffer);
}