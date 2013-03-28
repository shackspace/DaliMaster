#ifndef __DALI_ENCODE__
#define __DALI_ENCODE__

#include "dali_codes.h"
#include <avr/io.h>

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
#define _MODE_REPEAT_TWICE_    40
#define _MODE_QUERY_           42

#define dali_command_initialize_broadcast(output) dali_special_command(output, INITIALIZE, 0xFF)
#define dali_command_randomize(output) dali_special_command(output, RANDOMIZE)
#define dali_command_terminate(output) dali_special_command(output, RANDOMIZE)
#define dali_command_off(output,address) dali_slave_command(output,address,0x00) 

typedef enum {RANDOMIZE, INITIALIZE, TERMINATE} special_commands;

inline int dali_slave_direct_arc(word *output, byte address, byte brightness);

inline int dali_slave_command(word *output, byte address, byte command);

inline int dali_special_command(word *output, special_commands command, byte data);

#ifdef __cplusplus
}
#endif

#endif
