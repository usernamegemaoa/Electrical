#ifndef spi-AD7147_H
#define	spi-AD7147_H
//---------------------------------
//Function prototypes
//---------------------------------
void config_AD7147(void);
void write_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int DataBuffer[]);
void read_AD7147(const unsigned int RegisterAddress, unsigned int NumberOfRegisters, unsigned int *DataBuffer[]);
static int ServiceAD7147Isr(void);
#endif	/* XC_HEADER_TEMPLATE_H */

