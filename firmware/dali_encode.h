#ifndef __DALI_ENCODE__
#define __DALI_ENCODE__

#include "dali_codes.h"
#include <avr/io.h>

typedef unsigned short word;
typedef unsigned char  byte;

#define _ERR_WRONG_ADDRESS_    -1
#define _ERR_OK_                0
#define _ERR_WRONG_COMMAND_    -2
#define _ERR_RESERVED_COMMAND_ -3


typedef enum {RANDOMIZE, INITIALIZE} special_commands;

inline int dali_slave_direct_arc(word *output, byte address, byte brightness);

inline int dali_slave_commad(word *output, byte address, byte command);

inline int dali_special_command(word *output, special_commands command, byte data);

#endif
