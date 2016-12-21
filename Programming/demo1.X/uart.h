#ifndef UART_H
#define	UART_H

#include <xc.h>

void uart_write_char(char data);

void uart_write_int(int number);

void uart_write_unsigned_int(unsigned int number);

char uart_tx_empty(void);

void uart_write_string(const char *text);

char uart_int_to_string(int number);

void uart_write_hall_state(void);

void uart_write_direction(void);

void uart_write_voltage(int port);

char uart_data_ready(void);

char uart_read_char(void);

void uart_read_text(char *Output, unsigned int length);

#endif	/* UART_H */

