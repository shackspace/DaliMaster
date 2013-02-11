#include "suart.h"
#include "huart.h"
#include <util/delay.h>

void init( void )
{
  DDRD = 0;
  DDRB = (1 << 5);
  PORTD = 0xFF;
  PORTB = 0xFF;
  suart_init();
  uart_init();
}


int main( void )
{
  init();
  sei();
    
  int temp;
  for(;;){
    
    while((temp = suart_getc_nowait()) != -1)
	uart_putchar(temp);
    while(uart_charavailable())		
	suart_putc(uart_getchar());
  }
}



