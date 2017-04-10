#ifndef UART_H
#define	UART_H

#include <xc.h>

unsigned int cdc_signal(char, int);

void uart_write_char(char);

void uart_write_int(int);

void uart_write_unsigned_int(unsigned int);

char uart_tx_empty(void);

void uart_write_string(const char *text);

char uart_int_to_string(int number);

char uart_data_ready(void);

char uart_read_char(void);

void uart_read_text(char *Output, unsigned int length);

#endif	/* UART_H */
