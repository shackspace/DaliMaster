#include "Interpreter.h"
#include "dali_encode.h"
#include <ctype.h>

#ifdef TEST
#define _PROGMEM
#define strcmp_P strcmp
#define strcpy_P strcpy
#define pgm_read_byte(a) a
#else
#include <avr/pgmspace.h> 
#define _PROGMEM PROGMEM
#endif

typedef byte bool;
#define FALSE 0
#define TRUE !0

#define MAX_COMMAND_LENGTH 64

const char identifier_message[] _PROGMEM = "DALI Master\r\n";

const char group_postfix[] _PROGMEM = "_g";

//special commands
const char command_randomize[] _PROGMEM = "randomize";
const char command_initialize[] _PROGMEM = "initialize";

//set level directly
const char command_arc[] _PROGMEM = "arc";
const char command_arc_group[] _PROGMEM = "arc_g";

//arc commands
const char command_off[] _PROGMEM = "off";
const char command_up[] _PROGMEM = "up";
const char command_down[] _PROGMEM = "down";
const char command_step_up[] _PROGMEM = "step_up";
const char command_step_down[] _PROGMEM = "step_down";
const char command_max_level[] _PROGMEM = "max";
const char command_min_level[] _PROGMEM = "min";
const char command_step_off[] _PROGMEM = "step_down_off";
const char command_step_on[] _PROGMEM = "step_up_on";
const char command_reset[] _PROGMEM = "reset";
const char command_store_level_dtr[] _PROGMEM = "store_level_dtr";
const char command_store_dtr_max[] _PROGMEM = "store_dtr_max";
const char command_store_dtr_min[] _PROGMEM = "store_dtr_min";
const char command_store_dtr_system_failure[] _PROGMEM = "store_dtr_system_failure";
const char command_store_dtr_power_on[] _PROGMEM = "store_dtr_power_on";
const char command_store_dtr_fade_time[] _PROGMEM = "store_dtr_fade_time";
const char command_store_dtr_fade_rate[] _PROGMEM = "store_dtr_fade_rate";
const char command_store_dtr_scene[] _PROGMEM = "store_dtr_scene";
const char command_store_dtr_short_address[] _PROGMEM = "store_dtr_short_address";

//query
const char command_query_status[] _PROGMEM = "query_status";
const char command_query_ballast[] _PROGMEM = "query_ballast";
const char command_query_lamp_failure[] _PROGMEM = "query_lamp_failure";
const char command_query_lamp_power_on[] _PROGMEM = "query_power";
const char command_query_limit_error[] _PROGMEM = "query_limit_error";
const char command_query_reset_state[] _PROGMEM = "query_reset_state";
const char command_query_missing_short_address[] _PROGMEM = "query_missing_short_address";
const char command_query_version_number[] _PROGMEM = "query_version_number";
const char command_query_content_dtr[] _PROGMEM = "query_content_dtr";
const char command_query_device_type[] _PROGMEM = "query_device_type";
const char command_query_physical_minimum_level[] _PROGMEM = "query_physical_minimum_level";
const char command_query_power_failure[] _PROGMEM = "query_power_failure";
const char command_query_actual_level[] _PROGMEM = "query_actual_level";
const char command_query_max_level[] _PROGMEM = "query_max_level";
const char command_query_min_level[] _PROGMEM = "query_min_level";
const char command_query_power_on_level[] _PROGMEM = "query_power_on_level";
const char command_query_system_failure_level[] _PROGMEM = "query_system_failure_level";
const char command_query_fade_time[] _PROGMEM = "query_fade_time";


//commands with param
const char command_goto_scene[] _PROGMEM = "scene";
const char command_remove_from_scene[] _PROGMEM = "remove_scene";
const char command_add_to_group[] _PROGMEM = "add_group";
const char command_remove_from_group[] _PROGMEM = "remove_group";

//query with param

typedef struct key_value_mode	
{
	const char* key;
	byte value;
	byte mode;
} key_value_mode;

typedef struct key_special_mode	
{
	const char* key;
	special_command_type special;
	byte mode;
} key_special_mode;

#define COUNT_COMMANDS 7

const key_value_mode command_list[] = {
	{command_off, DALI_IMMEDIATE_OFF, _MODE_SIMPLE_}, 
	{command_up, DALI_UP_200MS, _MODE_SIMPLE_}, 
	{command_down, DALI_DOWN_200MS, _MODE_SIMPLE_}, 
	{command_step_up, DALI_STEP_UP, _MODE_SIMPLE_}, 
	{command_step_down, DALI_STEP_DOWN, _MODE_SIMPLE_},
	{command_max_level, DALI_RECALL_MAX_LEVEL, _MODE_SIMPLE_}, 
	{command_min_level, DALI_RECALL_MIN_LEVEL, _MODE_SIMPLE_}
	};

#define COUNT_COMMANDS_WITH_PARAM 1

const key_value_mode command_with_param_list[] = {
	{command_goto_scene, DALI_GO_TO_SCENE, _MODE_SIMPLE_}, 
	};

#define COUNT_SPECIAL_COMMANDS 2

const key_special_mode special_command_list[] = {
		{command_initialize, INITIALIZE, _MODE_REPEAT_TWICE_}, 
	{command_randomize, RANDOMIZE, _MODE_REPEAT_TWICE_}
	};



 

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

inline int parse_int(char* string, int16_t* integer)
{
	int16_t i = 0;
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
	
	if(!strcmp_P(command, command_arc))
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

	if(!strcmp_P(command, command_arc_group))
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
		if(!strcmp_P(command, command_list[i].key))
		{
			if(has_param1)
			{
				ret = dali_slave_command(output, (byte)param1, command_list[i].value);
				if(ret == _ERR_OK_)
					return pgm_read_byte(command_list[i].mode);
				else
					return ret;
			}
			return _ERR_PARSE_ERROR_;
		}
	}


	for(i = 0; i < COUNT_COMMANDS; i++)
	{
		strcpy_P(groupify, command_list[i].key);
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

	for(i = 0; i < COUNT_COMMANDS_WITH_PARAM; i++)
	{
		if(!strcmp_P(command, command_with_param_list[i].key))
		{
			if(has_param1)
			{
				ret = dali_slave_command(output, (byte)param1, command_with_param_list[i].value);
				if(ret == _ERR_OK_)
					return command_with_param_list[i].mode;
				else
					return ret;
			}
			return _ERR_PARSE_ERROR_;
		}
	}


	for(i = 0; i < COUNT_COMMANDS_WITH_PARAM; i++)
	{
		strcpy_P(groupify, command_with_param_list[i].key);
		strcat(groupify, group_postfix);
		if(!strcmp(groupify, command))
		{
			if(has_param1)
			{			
				ret = dali_group_command(output, (byte)param1, command_with_param_list[i].value);
				if(ret == _ERR_OK_)
					return command_with_param_list[i].mode;
				else
					return ret;
			}
			return _ERR_PARSE_ERROR_;
		}
	}

	for(i = 0; i < COUNT_SPECIAL_COMMANDS; i++)
	{
		if(!strcmp_P(command, special_command_list[i].key))
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
