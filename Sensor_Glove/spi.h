 #ifndef SPI_H
#define	SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  

void setup_spi(void);

unsigned int spi(unsigned int);

unsigned int cdc_signal(char, int);

#endif	/* XC_HEADER_TEMPLATE_H */

