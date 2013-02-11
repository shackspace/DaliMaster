#include "huart.h"
#define BAUD 115200
#include <util/setbaud.h>
#define TIMEOUT 250
 
void uart_init(void)   
{
   UBRR0H = UBRRH_VALUE;
   UBRR0L = UBRRL_VALUE;
  
#if USE_2X
   UCSR0A |= (1 << U2X0);
#else
   UCSR0A &= ~(1 << U2X0);
#endif
 

  UCSR0B |= (1<<TXEN0);
  // Frame Format: Asynchron 8N1
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
}

void uart_putchar(uint8_t data)
{
    // Make sure that the UART buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

uint8_t uart_charavailable()
{
    if (UCSR0A & (1 << RXC0))
	return 1;
    return 0;
}

uint8_t uart_getchar(void)
{
    uint8_t timeout = TIMEOUT;
     // Wait for RX to complete
    while ((!(UCSR0A & (1 << RXC0))) && timeout) {
	timeout--;
    }
    if (timeout) {
        return UDR0;
    } else {
        // No data, timeout
        return 0;
    }
}
