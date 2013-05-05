#include "dali.h"
#include "eusart_lib.h"
#include <util/delay.h>

void dali_init()
{
	eusart_clear();	

	eusart_init (MSK_EUSART_TX_16BIT, MSK_EUSART_RX_8BIT, MANCHESTER, MSB_FIRST, 16000000, 600, PARITY_NONE, 2, 2);

}

int dali_send(word frame)
{
	if(frame == INVALID_FRAME)
		return _ERR_INVALID_FRAME_;	
	eusart_put(frame);
	return _ERR_OK_;
}

int dali_send_with_repeat(word frame)
{
	if(frame == INVALID_FRAME)
		return _ERR_INVALID_FRAME_;	
	eusart_put(frame);
    	_delay_ms(40);
	eusart_put(frame);
	return _ERR_OK_;
}


int dali_query(word frame, byte* result)
{
	int i;
	if(frame == INVALID_FRAME)
		return _ERR_INVALID_FRAME_;	
	eusart_put(frame);
	_delay_ms(20);
	for(i = 0; i < 50; i++)
	{
		_delay_ms(1);
		if(eusart_rx_ready())
			*result = eusart_get();
	}
}
