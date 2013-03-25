#ifndef _UART_H_
#define _UART_H_

#include <avr/interrupt.h> 	

#define SUART_TXD
#define SUART_RXD

extern void suart_init();

#ifdef SUART_TXD
    extern void suart_putc (const char);
    extern void suart_putstring(char*);
#endif // SUART_RXD

#ifdef SUART_RXD
    extern int suart_getc_wait ();
    extern int suart_getc_nowait();
#endif // SUART_RXD

#endif /* _UART_H_ */

