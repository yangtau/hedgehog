#ifndef HG_STRING_H_
#define HG_STRING_H_

#include "common.h"

void hg_strtable_init();
void hg_strtable_free();
// copy `from`, and append return value to the global string_table
hg_str hg_str_new(const char* from, size_t len);

struct hg_strbuf {
    size_t len;
    size_t _cap;
    char* _str;
    bool _moved; // if the `_str` has been moved to string table
};

void hg_strbuf_init(struct hg_strbuf* buf);
void hg_strbuf_free(struct hg_strbuf* buf);
bool hg_strbuf_append(struct hg_strbuf* buf, const char* fmt, ...);
// change strbuf to str, and append it to the global string_table
hg_str hg_strbuf_to_str(struct hg_strbuf* buf);

#endif // HG_STRING_H_
