#include "dali.h"
#include "euart.h"
#include <util/delay.h>

void dali_init()
{
	euart_init();
}

int dali_send(word frame)
{
	euart_put(frame);
	return _ERR_OK_;
}

int dali_send_with_repeat(word frame)
{
	euart_put(frame);
    _delay_ms(40);
	euart_put(frame);
	return _ERR_OK_;
}


int dali_query(word frame, byte* result)
{
	int i;
	euart_put(frame);
	_delay_ms(20);
	for(i = 0; i < 50; i++)
	{
		_delay_ms(1);
		if(euart_rx_ready())
			*result = euart_get();
	}
}
