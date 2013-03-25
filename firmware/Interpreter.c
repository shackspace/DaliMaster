#include "Interpreter.h"
#include "dali_encode.h"

#ifdef TEST
#define _PROGMEM
#else
#define _PROGMEM __attribute__((__progmem__))
#endif

char const _PROGMEM identifier_message[] = "DALI Master\r\n";
char const _PROGMEM string_randomize[] = "randomize";
char const _PROGMEM string_initialize[] = "initialize";
char const _PROGMEM string_arc[] = "arc";

inline char nibble_to_ascii(uint8_t nibble)
{		
	nibble = nibble & 0x0F;
	if(nibble < 0x0A)
		return nibble + '0';
	else
		return nibble + 'A' - 0x0A;
}

inline char ascii_to_nibble(char nibble)
{		
	if(isdigit(nibble))
		return nibble - '0';
	else if(nibble >= 'A' && nibble <= 'F')
		return nibble - 'A' + 0x0A;
	else
		return -1;
}

int decode_command_to_frame(char* token, word* output)
{
	return _ERR_UNIMPLEMENTED_;
}

void get_substring(char* src, char *dest, unsigned int begin, unsigned int end)
{
    unsigned int i;
    for(i = 0; (i < (end - begin)) && (i < (strlen(src) - begin)); i++)
    {
        dest[i] = src[begin + i];
    }
    dest[i] = 0;
}


