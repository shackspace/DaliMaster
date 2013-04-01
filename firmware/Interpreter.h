#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string.h>
#include "dali_encode.h"
#include "fifo.h"

	
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_BUFFER_LENGTH 128
#define _ERR_PARSE_ERROR_ -253
#define _ERR_BUFFER_FULL_ -254
#define _ERR_PARAMETER_MISSING_ -255
#define _ERR_UNIMPLEMENTED_ -256

inline char nibble_to_ascii(uint8_t nibble);

inline char ascii_to_nibble(char nibble);

int decode_command_to_frame(char* token, word* output);

int parse_int(char* string, int16_t* integer);

#ifdef __cplusplus
}
#endif
#endif
