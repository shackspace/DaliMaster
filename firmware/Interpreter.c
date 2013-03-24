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


byte buffer[MAX_BUFFER_LENGTH];
unsigned int buffer_pos = 0;

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

int get_line(char* buffer, char* token)
{
    char temp[MAX_BUFFER_LENGTH];
    int i;
    for(i = 0; i < MAX_BUFFER_LENGTH && buffer[i] != 0; i++)
    {
        if(buffer[i] == '\n')
            break;
    }
    if(buffer[i] == 0 || i == MAX_BUFFER_LENGTH)
    {
        return -1;
    }
    else
    {
        get_substring(buffer, token, 0, i);
        get_substring(buffer, temp, i+1, MAX_BUFFER_LENGTH);
        strcpy(buffer, temp);
	buffer_pos = buffer_pos - i;
        return i;
    }
}

int decode_command_to_frame(char* token, word* output)
{
	return _ERR_UNIMPLEMENTED_;
}

int add_char_to_buffer(byte c)
{
	if(buffer_pos < MAX_BUFFER_LENGTH)
	{
		buffer[buffer_pos] = c;
		return _ERR_OK_;
	}		
	else
		return _ERR_BUFFER_FULL_;
}	

int clear_buffer()	
{
	int i;
	buffer_pos = 0;

	for(i = 0; i < MAX_BUFFER_LENGTH; i++)
	{
		buffer[i] = 0;
	}
	return _ERR_OK_;
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


