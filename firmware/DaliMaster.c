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
	byte buffer[MAX_BUFFER_LENGTH] = {[0 ... MAX_BUFFER_LENGTH-1] = 0};
	fifo_t fifo;	
	word frame;
	int ret;

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
			_inline_fifo_get_chars( &fifo, temp, fifo.count);
			if(ret = decode_command_to_frame(temp, &frame) >= 0)
			{
				if(_ERR_OK_ == ret)
				{ 
					dali_send(frame);
					suart_putstring("ACK\n");
				}
				else if(_MODE_REPEAT_TWICE_ == ret)
				{
					dali_send_with_repeat(frame);
					suart_putstring("ACK\n");
				}
				else if(_MODE_QUERY_ == ret)
				{
					byte ans;
					dali_query(frame, &ans);
					suart_putstring("ANS ");
					suart_putc(nibble_to_ascii(ans >> 4));
					suart_putc(nibble_to_ascii(ans & 0x0F));
					suart_putc('\n');
				}
			}
			else
			{
				suart_putstring("NACK\n");
			}
		}		
	}
	
	return 0;
}
