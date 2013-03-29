#ifndef _DALI_H_
#define _DALI_H_

#include "dali_codes.h"
#include "dali_encode.h"

#define _ERR_NO_ANSWER_ -100

#ifdef __cplusplus
extern "C"
{
#endif

void dali_init();

int dali_send(word frame);

int dali_send_with_repeat(word frame);

int dali_query(word frame, byte* result);

#ifdef __cplusplus
}
#endif

#endif
