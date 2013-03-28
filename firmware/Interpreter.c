#include "Interpreter.h"
#include "dali_encode.h"

#ifdef TEST
#define _PROGMEM
#else
#define _PROGMEM __attribute__((__progmem__))
#endif

char const _PROGMEM identifier_message[] = "DALI Master\r\n";

//special commands
char const _PROGMEM command_randomize[] = "randomize";
char const _PROGMEM command_initialize[] = "initialize";

//set level directly
char const _PROGMEM command_arc[] = "arc";

//arc commands
char const _PROGMEM command_up[] = "up";
char const _PROGMEM command_down[] = "down";
char const _PROGMEM command_step_up[] = "step_up";
char const _PROGMEM command_step_down[] = "step_down";
char const _PROGMEM command_goto_scene[] = "scene";
char const _PROGMEM command_max_level[] = "max";
char const _PROGMEM command_min_level[] = "min";

typedef struct key_value_mode	
{
	char* key;
	byte value;
	byte mode;
} key_value;

key_value arc_commands[] = {{command_up, DALI_UP_200MS, _MODE_SIMPLE_}, {command_down, DALI_DOWN_200MS, _MODE_SIMPLE_}, 
	{command_step_up, DALI_STEP_UP, _MODE_SIMPLE_}, {command_step_down, DALI_STEP_DOWN, _MODE_SIMPLE_},
	{command_goto_scene, DALI_GO_TO_SCENE, _MODE_SIMPLE_},
	{command_max_level, DALI_RECALL_MAX_LEVEL, _MODE_SIMPLE_}, {command_min_level, DALI_RECALL_MIN_LEVEL, _MODE_SIMPLE_}
	};

#define MAX_COMMAND_LENGTH 64

 

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

inline int parse_int(char* string)
{
	int i = 0;
	byte sign = 0;
	
	if(*string == '-')
	{	
		sign = !0;
		string++;
	}

	if(isdigit(*string) && (*string != 0))
	{
		i *= 10;
		i += atoi(*string);
	}
	else if(sign)
	{
		return -i;
	}
	else
	{
		return i;
	}
}

int decode_command_to_frame(char* token, word* output)
{
	char* command[MAX_COMMAND_LENGTH];
	uint16_t i;
	const uint16_t length = strlen(token);
	for(i = 0; (i < length) && (token[i] == ' '); i++); //discard leading spaces 
		
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


