#include "startup_values.h"
#include "dali.h"

#ifdef TEST
word temp;
#define uint8_t unsigned char
#define PROGMEM
#define EEMEM
#define strcmp_P strcmp
#define strcpy_P strcpy
#define eeprom_read_byte(a) a
#define eeprom_update_byte(a,b) *a = b
#define dali_send(a) temp=a 
#else
#include <avr/eeprom.h> 
#endif

#ifndef TEST
#include <util/delay.h>
#endif


uint8_t arc_device[64] = {[0 ... 63] = 255};
uint8_t arc_group[16] = {[0 ... 15] = 255};

uint8_t arc_device_eeprom[64] EEMEM = {[0 ... 63] = 255};
uint8_t arc_group_eeprom[16] EEMEM = {[0 ... 15] = 255};

void load_startup_values()
{
	int i;
	for(i = 0; i < 64; i++)
	{
		arc_device[i] = eeprom_read_byte (&arc_device_eeprom[i]);
	}
	for(i = 0; i < 16; i++)
	{
		arc_group[i] = eeprom_read_byte (&arc_group_eeprom[i]);
	}
}

void save_startup_values()
{
	int i;
	for(i = 0; i < 64; i++)
	{
		eeprom_update_byte(&arc_device_eeprom[i], arc_device[i]);
	}
	for(i = 0; i < 16; i++)
	{
		eeprom_update_byte(&arc_group_eeprom[i], arc_group[i]);
	}
}

void clear_startup_values()
{
	int i;
	for(i = 0; i < 64; i++)
	{
		arc_device[i] = 0xFF;
		eeprom_update_byte(&arc_device_eeprom[i], arc_device[i]);
	}
	for(i = 0; i < 16; i++)
	{
		arc_group[i] = 0xFF;
		eeprom_update_byte(&arc_group_eeprom[i], arc_group[i]);
	}
}

void send_startup_values()
{
	int i;
	word frame;
	for(i = 0; i < 64; i++)
	{
		if(arc_device[i] != 255)
		{
			dali_slave_direct_arc(&frame, i, arc_device[i]);
			dali_send(frame);
#ifndef TEST
			_delay_ms(40);
#endif
		}
	}
	for(i = 0; i < 16; i++)
	{
		if(arc_group[i] != 255)
		{
			dali_group_direct_arc(&frame, i, arc_group[i]);
			dali_send(frame);
#ifndef TEST
			_delay_ms(40);
#endif
		}
	}
}

