#include "Interpreter.h"
#include "dali_encode.h"

#ifdef TEST
#define _PROGMEM
#else
#define _PROGMEM __attribute__((__progmem__))
#endif

typedef byte bool;
#define FALSE 0
#define TRUE !0

const char _PROGMEM identifier_message[] = "DALI Master\r\n";

const char _PROGMEM group_postfix[] = "_g";

//special commands
const char _PROGMEM command_randomize[] = "randomize";
const char _PROGMEM command_initialize[] = "initialize";

//set level directly
const char _PROGMEM command_arc[] = "arc";
const char _PROGMEM command_arc_group[] = "arc_g";

//arc commands
const char _PROGMEM command_up[] = "up";
const char _PROGMEM command_down[] = "down";
const char _PROGMEM command_step_up[] = "step_up";
const char _PROGMEM command_step_down[] = "step_down";
const char _PROGMEM command_goto_scene[] = "scene";
const char _PROGMEM command_max_level[] = "max";
const char _PROGMEM command_min_level[] = "min";

typedef struct key_value_mode	
{
	char* key;
	byte value;
	byte mode;
} key_value_mode;

typedef struct key_special_mode	
{
	char* key;
	special_command_type special;
	byte mode;
} key_special_mode;

#define COUNT_COMMANDS 7

key_value_mode command_list[] = {{command_up, DALI_UP_200MS, _MODE_SIMPLE_}, 
	{command_down, DALI_DOWN_200MS, _MODE_SIMPLE_}, 
	{command_step_up, DALI_STEP_UP, _MODE_SIMPLE_}, 
	{command_step_down, DALI_STEP_DOWN, _MODE_SIMPLE_},
	{command_goto_scene, DALI_GO_TO_SCENE, _MODE_SIMPLE_},
	{command_max_level, DALI_RECALL_MAX_LEVEL, _MODE_SIMPLE_}, 
	{command_min_level, DALI_RECALL_MIN_LEVEL, _MODE_SIMPLE_}
	};

#define COUNT_SPECIAL_COMMANDS 2

key_special_mode special_command_list[] = {{command_initialize, INITIALIZE, _MODE_REPEAT_TWICE_}, 
	{command_randomize, RANDOMIZE, _MODE_REPEAT_TWICE_}
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

inline int parse_int(char* string, int* integer)
{
	int i = 0;
	bool sign = FALSE;
	
	if(*string == '-')
	{	
		sign = TRUE;
		string++;
	}	

	while(*string != 0)
	{			
		if(isdigit(*string) && (*string != 0))
		{
			i *= 10;
			i += *string - '0';
			string++;
		}
		else
		{
			return _ERR_PARSE_ERROR_;
		}
	}
	
	if(sign)
	{
		*integer = -i;
	}
	else
	{
		*integer = i;
	}
		
	
	return _ERR_OK_;
}

int decode_command_to_frame(char* token, word* output)
{
	char command[MAX_COMMAND_LENGTH+1] = {0};
	char groupify[MAX_COMMAND_LENGTH] = {0};
	char param1_string[MAX_COMMAND_LENGTH+1] = {0};
	char param2_string[MAX_COMMAND_LENGTH+1] = {0};
	bool has_param1 = FALSE;
	bool has_param2 = FALSE;
	int16_t param1 = 0;
	int16_t param2 = 0;
	uint16_t i;
	uint16_t u;
	int ret;
	const uint16_t length = strlen(token);
	if(length == 0)
		return _ERR_PARSE_ERROR_;
	for(i = 0; (i < length) && (token[i] == ' '); i++); //discard leading spaces 

	u = i;

	for(; (i < length) && (token[i] != ' ') && (token[i] != '\n')	; i++)
	{
		if(i-u >= MAX_COMMAND_LENGTH)
			return _ERR_PARSE_ERROR_;
		command[i-u] = token[i];
	}

	for(; (i < length) && (token[i] == ' '); i++); //discard leading spaces 

	u = i;

	for(; (i < length) && (token[i] != ' ') && (token[i] != '\n'); i++)
	{
		has_param1 = TRUE;
		if(i-u >= MAX_COMMAND_LENGTH)
			return _ERR_PARSE_ERROR_;
		param1_string[i-u] = token[i];
	}

	for(; (i < length) && (token[i] == ' '); i++); //discard leading spaces 

	u = i;

	for(; (i < length) && (token[i] != ' ') && (token[i] != '\n'); i++)
	{
		has_param2 = TRUE;
		if(i-u >= MAX_COMMAND_LENGTH)
			return _ERR_PARSE_ERROR_;
		param2_string[i-u] = token[i];
	}

	if(has_param1)
	{
		if(_ERR_OK_ != parse_int(param1_string, &param1))
			return _ERR_PARSE_ERROR_;
		if(param1 > 255 || param1 < 0)
			return _ERR_PARSE_ERROR_;
	}
	if(has_param2)
	{
		if(_ERR_OK_ !=  parse_int(param2_string, &param2))
			return _ERR_PARSE_ERROR_;
		if(param2 > 255 || param2 < 0)
			return _ERR_PARSE_ERROR_;
	}
	
	if(!strcmp(command_arc, command))
	{
		if(has_param1 && has_param2)
		{
			ret = dali_slave_direct_arc(output, (byte)param1, (byte)param2);
			if(ret == _ERR_OK_)
				return _MODE_SIMPLE_;
			else return ret;
		}
		return _ERR_PARSE_ERROR_;
	}

	if(!strcmp(command_arc_group, command))
	{
		if(has_param1 && has_param2)
		{
			ret = dali_group_direct_arc(output, (byte)param1, (byte)param2);
			if(ret == _ERR_OK_)
				return _MODE_SIMPLE_;
			else return ret;
		}
		return _ERR_PARSE_ERROR_;
	}


	for(i = 0; i < COUNT_COMMANDS; i++)
	{
		if(!strcmp(command_list[i].key, command))
		{
			if(has_param1)
			{
				ret = dali_slave_command(output, (byte)param1, command_list[i].value);
				if(ret == _ERR_OK_)
					return command_list[i].mode;
				else
					return ret;
			}
			return _ERR_PARSE_ERROR_;
		}
	}


	for(i = 0; i < COUNT_COMMANDS; i++)
	{
		strcpy(groupify, command_list[i].key);
		strcat(groupify, group_postfix);
		if(!strcmp(groupify, command))
		{
			if(has_param1)
			{			
				ret = dali_group_command(output, (byte)param1, command_list[i].value);
				if(ret == _ERR_OK_)
					return command_list[i].mode;
				else
					return ret;
			}
			return _ERR_PARSE_ERROR_;
		}
	}

	for(i = 0; i < COUNT_SPECIAL_COMMANDS; i++)
	{
		if(!strcmp(special_command_list[i].key, command))
		{
			ret = dali_special_command(output, special_command_list[i].special, (byte)param1);
			if(ret == _ERR_OK_)
				return special_command_list[i].mode;
			else
				return ret;
		}
	}
	
		
	return _ERR_UNIMPLEMENTED_;
}

/*
void get_substring(char* src, char *dest, unsigned int begin, unsigned int end)
{
    unsigned int i;
    for(i = 0; (i < (end - begin)) && (i < (strlen(src) - begin)); i++)
    {
        dest[i] = src[begin + i];
    }
    dest[i] = 0;
}
*/


