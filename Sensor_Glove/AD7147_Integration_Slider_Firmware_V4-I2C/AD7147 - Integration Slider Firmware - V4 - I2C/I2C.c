/********************************************************************************************************/
/* I2C inteface for ADUC841																				*/
/* ------------------------																				*/
/*																										*/
/* Description.																							*/
/* This file has the function to handle the I2C protocol using the ADuC841. In this case, the ADuC841 	*/
/* is the master.																						*/
/*																										*/
/* Date: 16/05/2005																						*/
/* Author: ADI																							*/
/********************************************************************************************************/
#include <ADUC841.h>
#include <intrins.h>
#include "Include\General Definitions.h"

#define AD7147_ADD		0x2C
#define I2C_WR			0
#define I2C_RD			1
#define	ACK				0
#define	NACK			1


//---------------------------------
//Function prototypes
//---------------------------------
//High level functions
BYTE WriteToAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToWrite, WORD *DataBuffer, const WORD OffsetInBuffer);
BYTE ReadFromAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToRead, WORD *DataBuffer, const WORD OffsetInBuffer);
//Low level functions
void InitialiseI2CMaster(void);
void StartI2CMaster(void);
void StopI2CMaster(void);
BYTE SendByteI2CMaster(BYTE ByteToSend);
BYTE ReceiveByteI2CMaster(bit ackn);

//---------------------------------
//Function definitions
//---------------------------------

/********************************************************************************************************/
/****************** High level functions, need to be customised according to the device *****************/
/********************************************************************************************************/

//---------------------------------
//WriteAD7147ViaI2C()
//---------------------------------
//Function that writes to the AD7147 via the I2C port. It sends first the device
//address including the write bit, then the register address and finally the
//data. The function returns "1" if successfull otherwise "0".
//--------------------------------------------------------------------------------
BYTE WriteToAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToWrite, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	WORD DataToWrite;
	BYTE LowByteAddress, HighByteAddress;
	BYTE LowByteData, HighByteData;
	BYTE r, AcknError;
	BYTE DeviceAddress;

	AcknError=1; //No error on initialisation

	//Add the write bit to the device address
	DeviceAddress = AD7147_ADD<<1 | I2C_WR;
	//Split the address in two bytes 
	HighByteAddress = (RegisterStartAddress & 0xFF00) >>8;
	LowByteAddress = RegisterStartAddress & 0x00FF;

	//Start the I2C transfer
	InitialiseI2CMaster();
	StartI2CMaster();
	//Send device address
	if(!SendByteI2CMaster(DeviceAddress))
	{
		//Send register address if the acknowledgement is there
		if (!SendByteI2CMaster(HighByteAddress))
		{
			if (!SendByteI2CMaster(LowByteAddress))
			{
				//Perform block write
				for (r=0;r<NumberOfRegistersToWrite;r++)
				{
					DataToWrite = DataBuffer[OffsetInBuffer+r];
					LowByteData = DataToWrite & 0x00FF;
					HighByteData = (DataToWrite & 0xFF00)>>8;
					if(!SendByteI2CMaster(HighByteData))
					{
						SendByteI2CMaster(LowByteData);
					}
					else //No acknowledgement was found therefore send the stop condition
					{
						StopI2CMaster();
						AcknError=0;
					}
				}
				//Stop transfer
				StopI2CMaster();
			}
			else //No acknowledgement was found therefore send the stop condition
			{
				StopI2CMaster();
				AcknError=0;
			}
		}
		else //No acknowledgement was found therefore send the stop condition
		{
			StopI2CMaster();
			AcknError=0;
		}
	}
	else //No acknowledgement was found therefore send the stop condition
	{
		StopI2CMaster();
		AcknError=0;
	}
	return(AcknError);
}


//---------------------------------
//ReadFromAD7147ViaI2C()
//---------------------------------
//Function that reads from the AD7147 via the I2C port. It sends first the device
//address including the write bit, then the register address and finally reads data
//back. The function returns "1" if successfull otherwise "0". If an error occurs,
//Then the stop condition is sent.
//--------------------------------------------------------------------------------
BYTE ReadFromAD7147ViaI2C(const WORD RegisterStartAddress, const BYTE NumberOfRegistersToRead, WORD *DataBuffer, const WORD OffsetInBuffer)
{
	WORD ReadData;
	BYTE LowByteAddress, HighByteAddress;
	BYTE LowByteData, HighByteData;
	BYTE r, AcknError;
	BYTE DeviceAddress;

	AcknError=1; //No error on initialisation
	
	//Add the write bit to the device address
	DeviceAddress=AD7147_ADD<<1 | I2C_WR;
	//Split the address in two bytes 
	HighByteAddress = (RegisterStartAddress & 0xFF00) >>8;
	LowByteAddress = RegisterStartAddress & 0x00FF;

	//Start the I2C transfer
	InitialiseI2CMaster();
	StartI2CMaster();

	//Send device address
	if(!SendByteI2CMaster(DeviceAddress))
	{
		//Send register address
		if(!SendByteI2CMaster(HighByteAddress))
		{
			if(!SendByteI2CMaster(LowByteAddress))
			{
				//Send the repeated start
				StartI2CMaster();
				//Send device address again changing the Rd/Wr bit
				DeviceAddress = AD7147_ADD<<1 | I2C_RD;
				if(!SendByteI2CMaster(DeviceAddress))
				{
					//Perform block read, but first,we need to know if we must send an ACKN or a NACK
					if(NumberOfRegistersToRead==1)
					{
						HighByteData=ReceiveByteI2CMaster(ACK);
						LowByteData=ReceiveByteI2CMaster(NACK);
						ReadData=((HighByteData & 0xFF)<<8) | LowByteData;
						DataBuffer[OffsetInBuffer]=ReadData;
					}
					else
					{
						for (r=0;r<(NumberOfRegistersToRead-1);r++)
						{
							HighByteData=ReceiveByteI2CMaster(ACK);
							LowByteData=ReceiveByteI2CMaster(ACK);
							ReadData=((HighByteData & 0xFF)<<8) | LowByteData;	//Recombine 16-bit data
							DataBuffer[OffsetInBuffer+r]=ReadData;
						}
						//Do the last read sending the NACK
						HighByteData=ReceiveByteI2CMaster(ACK);
						LowByteData=ReceiveByteI2CMaster(NACK);
						ReadData=((HighByteData & 0xFF)<<8) | LowByteData;	//Recombine 16-bit data
						DataBuffer[OffsetInBuffer+NumberOfRegistersToRead-1]=ReadData;
					}
					//Stop transfer
					StopI2CMaster();
				}
				else //No acknowledgement was found therefore send the stop condition
				{
					StopI2CMaster();
					AcknError=0;
				}
			}
			else //No acknowledgement was found therefore send the stop condition
			{
				StopI2CMaster();
				AcknError=0;
			}
		}
		else //No acknowledgement was found therefore send the stop condition
		{
			StopI2CMaster();
			AcknError=0;
		}
	}
	else //No acknowledgement was found therefore send the stop condition
	{
		StopI2CMaster();
		AcknError=0;
	}
	return(AcknError);
}



/********************************************************************************************************/
/*** Low level functions, do not change anything below this line, however check the valid clock level ***/
/********************************************************************************************************/

//---------------------------------
//InitialiseI2CMaster();
//---------------------------------
//Function that configures the I2C port of the ADuC841 in master mode.
//--------------------------------------------------------------------------------
void InitialiseI2CMaster(void)
{
	ADD0=1;				//Write a "1" to ADD0 so that it becomes an input.
	CLK=1;				//Write a "1" to CLK so that it becomes an input.

	SPICON &= 0xDF;		//SPI interface is turned off by clearing the SPE bit.
	I2CCON = 0xA8;		//Master mode
}

//---------------------------------
//InitialiseI2CMaster();
//---------------------------------
//Function that configures the I2C port of the ADuC841 in master mode.

//---------------------------------
//StartI2CMaster();
//---------------------------------
//Function that implements the start condition of the I2C protocol. The start
//condition consists in a falling edge on SDA when SCL is high.
//--------------------------------------------------------------------------------
void StartI2CMaster(void)
{
	MDE = 1;	//SDA output enabled

	MCO = 0;	//SCL low
	MDO = 0;	//SDA low

	MDO = 1;	//SDA high
	MCO = 1;	//SCL high
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MDO = 0;	//SDA goes low before the clock
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MCO = 0;	//SCL low
	_nop_();
}



//---------------------------------
//StopI2CMaster();
//---------------------------------
//Function that implements the stop condition of the I2C protocol. The stop
//condition consists in a rising edge on SDA when SCL is high.
//--------------------------------------------------------------------------------
void StopI2CMaster(void)
{
	MDE = 1;	//SDA output enabled
	MDO = 0;	//SDA low
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MCO = 1;	//SCL high
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MDO = 1;	//SDA goes from low to high when SCL is already high,
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


//---------------------------------
//SendByteI2CMaster();
//---------------------------------
//Function that sends a byte to the I2C port and then read the acknowledgement 
//bit. If the acknowledgement is found, then the function returns "1" otherwise, 
//it returns "0".
//--------------------------------------------------------------------------------
BYTE SendByteI2CMaster(BYTE ByteToSend)
{
	BYTE i;
	BYTE noack;

	MDE = 1;	//Enable SDA output
	for (i=8; i>0; i--)
	{
		MCO = 0;      			//Reset SCL
		MDO = ByteToSend >> 7;	//Send data to SDA pin
		_nop_();
		MCO = 1;      			//Set SCL
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
		_nop_();
		_nop_();
		MCO = 0;      			//Reset SCL
		_nop_();
		ByteToSend<<=1;      	//Rotate data
	}
	MDE = 0;				//SDA becomes an input for the ACKN
	MCO = 0;				//Reset SCL

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

	MCO = 1;        		//Set SCL
	noack = (BYTE)MDI;		//Check SDA for ACKN
	MDO=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MCO = 0;
	MDE = 1;	//Enable SDA output
	return(noack);
}



//---------------------------------
//ReceiveByteI2CMaster();
//---------------------------------
//Function that reads one byte from the I2C port. If we do continuous read, 
//then the acknowledgement must be "0" excepted for the last read sequence which
//it must be "1".
//--------------------------------------------------------------------------------
BYTE ReceiveByteI2CMaster(bit ackn)
{
	unsigned char i;
	BYTE ReceivedByte;

	MDE = 0;	//Make SDA an input
	MCO = 0;	//Reset SCL
	for (i=8; i>0; i--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		MCO = 1;				//Set SCL
		ReceivedByte <<= 1;		//Rotate data
		ReceivedByte |= MDI;	//Read SDA -> data
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		MCO = 0;				//Reset SCL
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
	MDE = 1;	//SDA is turned in an output to write the ACK on the data line
	MDO = ackn;	//SDA = ACK bit
	_nop_();		
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MCO = 1;	//Set SCL
	_nop_();		
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	MCO = 0;	//Reset SCL
	_nop_();		
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
//	MDE = 0;	//SDA is an input again
	return(ReceivedByte);
}