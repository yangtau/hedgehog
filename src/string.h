#ifndef HG_STRING_H_
#define HG_STRING_H_

#include "common.h"

void hg_string_init();
void hg_string_free();
// copy `from`, and append return value to the global string_table
hg_char hg_string_new(const char* from, size_t len);

struct hg_string_buffer {};
struct hg_string_buffer* hg_string_buffer_new(size_t cap);
bool hg_string_buffer_append(struct hg_string_buffer*, const char* fmt, ...);
// change string_buffer to str, and append it to the global string_table
hg_char hg_string_buffer_to_str(struct hg_string_buffer*);

#endif // HG_STRING_H_
