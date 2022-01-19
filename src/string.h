#ifndef HG_STRING_H_
#define HG_STRING_H_

#include "common.h"

void hg_string_init();
void hg_string_free();
char* hg_string_new(char* from, size_t len);

#endif // HG_STRING_H_
