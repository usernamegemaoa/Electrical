/********************************************************************************************************/
/* SPI inteface for ADUC841																				*/
/* ------------------------																				*/
/*																										*/
/* Description.																							*/
/* This file has the function to handle the SPI interface between the ADuC841 and the AD7142.			*/
/*																										*/
/* Date: 29/07/2005																						*/
/* Author: ADI																							*/
/********************************************************************************************************/

//---------------------------------
//Function prototypes
//---------------------------------
void WriteToAD7147ViaSpi(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer);
void ReadFromAD7147ViaSpi(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer);


//---------------------------------
//Function definitions
//---------------------------------

//---------------------------------
//WriteRegistersToAD7147();
//---------------------------------
//This function implements block writes to the AD7147. We first send the control 
//word that includes the start address and then the data to write.
//--------------------------------------------------------------------------------
void WriteToAD7147ViaSpi(const WORD RegisterAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const BYTE OffsetInBuffer)
{
	WORD ControlValue;
	WORD ValueToWrite;
	WORD RegisterIndex;
	BYTE i;

	//Create the 16-bit header
	ControlValue = 0xE000 | (RegisterAddress & 0x03FF);
	
	CS=0; //bring CS low
	//Write out the control word
	for (i=0; i<=REGISTER_LENGTH; i++)
	{
		CLK=1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		ControlValue=ControlValue<<1;
		DOUT=CY; //send the MSB first.
		CLK=0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
	//And then the data
	for (RegisterIndex=0; RegisterIndex<NumberOfRegisters; RegisterIndex++)
	{
		ValueToWrite= *(DataBuffer+RegisterIndex+OffsetInBuffer);
		for (i=0; i<REGISTER_LENGTH; i++)
		{
			ValueToWrite=ValueToWrite<<1;
			DOUT=CY;//send the MSB first.
			CLK=0;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			CLK=1;
			_nop_();
			_nop_();
			_nop_();
		}
	}
	DOUT=1;
	CLK=1;
	CS=1; //bring CS high again
}


//---------------------------------
//ReadRegistersFromAD7147();
//---------------------------------
//This function implements a block read from the AD7142. We first send the 
//control word that includes the start address and then 16 clocks for each 
//registers to read.
//--------------------------------------------------------------------------------
void ReadFromAD7147ViaSpi(const WORD RegisterStartAddress, const BYTE NumberOfRegisters, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	WORD ControlValue;
	BYTE i, RegisterIndex;
	BYTE InputPinValue;
	WORD InputValue;

	//Create the 16-bit header
	ControlValue = 0xE400 | (RegisterStartAddress & 0x03FF);
	
	CS=0; //bring CS low

	//Write out the control word
	for (i=0; i<=REGISTER_LENGTH; i++)
	{
		CLK=1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		ControlValue=ControlValue<<1;
		DOUT=CY;//(OutputValue<<1); //send the MSB first.
		CLK=0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
	//Copy each register in the buffer contiguously.
	//Read data in.
	for (RegisterIndex=0; RegisterIndex<NumberOfRegisters; RegisterIndex++)
	{
		for (i=0; i<16; i++)
		{
			CLK=0;
			_nop_();
			InputPinValue=DIN &0x1;
			InputValue = InputValue <<1;
			InputValue |= InputPinValue;
			CLK=1;
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
		*(DataBuffer+OffsetInBuffer+RegisterIndex)=(WORD)InputValue;
	}
	DOUT=1;
	CS=1; //bring CS high again
}