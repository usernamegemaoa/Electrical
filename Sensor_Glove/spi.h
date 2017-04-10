#ifndef SPI_H
#define	SPI_H
 
void setup_spi(void);

void w32_spi(unsigned int, unsigned int);

void w16_spi(unsigned int);

unsigned int r16_spi(void);

unsigned int w16_r16_spi(unsigned int);

#endif	/* XC_HEADER_TEMPLATE_H */