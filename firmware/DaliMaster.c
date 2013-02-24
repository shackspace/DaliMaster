#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "euart.h"

int main()
{
	euart_init();
	
	return 0;
}
