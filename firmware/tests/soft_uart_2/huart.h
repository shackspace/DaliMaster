#ifndef _HARDWARE_UART_H_
#define _HARDWARE_UART_H_

#include <avr/io.h>

void uart_init(void);

void uart_putchar(uint8_t data);

uint8_t uart_charavailable();

uint8_t uart_getchar(void);

#endif
