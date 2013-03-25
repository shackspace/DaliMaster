#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "dali.h"
#include "suart.h"
#include "Interpreter.h"

int main()
{
	//input buffer for terminal
	byte buffer[MAX_BUFFER_LENGTH] = {0};
	fifo_t fifo;	

	//buffer for temporary strings
	byte temp[MAX_BUFFER_LENGTH];

	suart_init();
	dali_init();
	fifo_init(&fifo, buffer, MAX_BUFFER_LENGTH);


	for(;;)
	{
		byte c = suart_getc_wait () & 0xFF;
	  	fifo_put(&fifo, c);
		if(c == '\n') //'\n' is end of command
		{
			
		}		
	}
	
	return 0;
}
