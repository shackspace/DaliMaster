#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string.h>
#include "dali_encode.h"
#include "fifo.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_BUFFER_LENGTH 256
#define _ERR_PARSE_ERROR_ -253
#define _ERR_BUFFER_FULL_ -254
#define _ERR_UNIMPLEMENTED_ -256

int decode_command_to_frame(char* token, word* output);

void get_substring(char* src, char* dest, unsigned int begin, unsigned int end);

#ifdef __cplusplus
}
#endif
#endif
