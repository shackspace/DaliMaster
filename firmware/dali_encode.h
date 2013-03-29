#ifndef __DALI_ENCODE__
#define __DALI_ENCODE__

#include "dali_codes.h"
#ifndef TEST
#include <avr/io.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned short word;
typedef unsigned char  byte;

#define _ERR_WRONG_ADDRESS_    -1
#define _ERR_OK_                0
#define _ERR_WRONG_COMMAND_    -2
#define _ERR_RESERVED_COMMAND_ -3
#define _MODE_SIMPLE_          40
#define _MODE_REPEAT_TWICE_    41
#define _MODE_QUERY_           42

#define dali_command_initialize_broadcast(output) dali_special_command(output, INITIALIZE, 0xFF)
#define dali_command_randomize(output) dali_special_command(output, RANDOMIZE, 0)
#define dali_command_terminate(output) dali_special_command(output, RANDOMIZE, 0)
#define dali_command_off(output,address) dali_slave_command(output,address,0x00) 

typedef enum {RANDOMIZE, INITIALIZE, TERMINATE, COMPARE, WITHDRAW, PROGRAM_SHORT_ADDRESS, STORE_DTR} special_command_type;

int dali_slave_direct_arc(word *output, byte address, byte brightness);

int dali_group_direct_arc(word *output, byte address, byte brightness);

int dali_slave_command(word *output, byte address, byte command);

int dali_group_command(word *output, byte address, byte command);

int dali_slave_command_with_param(word *output, byte address, byte command, byte param);

int dali_group_command_with_param(word *output, byte address, byte command, byte param);

int dali_special_command(word *output, special_command_type command, byte data);

#ifdef __cplusplus
}
#endif

#endif
