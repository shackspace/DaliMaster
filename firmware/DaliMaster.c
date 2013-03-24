#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "dali.h"
#include "suart.h"
#include "Interpreter.h"

int main()
{
	suart_init();
	dali_init();

	byte temp[MAX_BUFFER_LENGTH];

	for(;;)
	{
		byte c = suart_getc_wait () & 0xFF;
	  	if(add_char_to_buffer(c) != _ERR_BUFFER_FULL_);
		{
			clear_buffer();
			continue;	
		}
		
			
		
	}

	
	return 0;
}
