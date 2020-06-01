#ifndef _HG_STRING_H_
#define _HG_STRING_H_
#include "common.h"
#include "object.h"

//> hg_string
struct hg_string {
    struct hg_object obj;
    size_t len;
    char str[];
};
// hg_string_copy: copy `s` to create a new hg_string
struct hg_string* hg_string_copy(const char* s, size_t len);
//< hg_string

//> hg_symbol
struct hg_string* hg_symbol_copy(const char* s, size_t len);
//< hg_symbol
#endif // _HG_STRING_H_
