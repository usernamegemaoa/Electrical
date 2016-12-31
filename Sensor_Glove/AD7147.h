#ifndef AD7147_H
#define	AD7147_H
//---------------------------------
//Function prototypes
//---------------------------------
void setup_AD7147(void);
void write_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int DataBuffer[]);
void read_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int *DataBuffer[]);
static int ServiceAD7147Isr(void);
unsigned int read_Sensor(int sensor);
#endif	/* XC_HEADER_TEMPLATE_H */


