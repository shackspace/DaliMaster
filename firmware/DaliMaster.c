#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "dali.h"

int main()
{
	dali_init();
	
	return 0;
}
