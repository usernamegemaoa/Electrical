#ifndef SPI_H
#define	SPI_H
 
void setup_spi(void);

void write_spi(unsigned int message);

void write_spi_solo(unsigned int message);

unsigned int read_spi(void);

unsigned int read_spi_solo(void);

unsigned int cdc_signal(char, int);

#endif	/* XC_HEADER_TEMPLATE_H */