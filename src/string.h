#ifndef HG_STRING_H_
#define HG_STRING_H_

#include "common.h"

void hg_string_init();
void hg_string_free();
// copy `from`, and append return value to the global string_table
hg_char hg_string_new(const char* from, size_t len);

struct hg_string_buffer {
    size_t len;
    size_t _cap;
    char* _str;
    bool _moved; // if the `_str` has been moved to string table
};
void hg_string_buffer_init(struct hg_string_buffer* buf, size_t cap);
void hg_string_buffer_free(struct hg_string_buffer* buf);
bool hg_string_buffer_append(struct hg_string_buffer* buf, const char* fmt,
                             ...);
// change string_buffer to str, and append it to the global string_table
hg_char hg_string_buffer_to_str(struct hg_string_buffer* buf);

#endif // HG_STRING_H_
