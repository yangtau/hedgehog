#include "string.h"
#include "memory.h"

#include <string.h>

static struct {
    hg_char* arr;
    size_t len;
    size_t cap;
} _string_table;

#ifdef HG_DEBUG
static const size_t _start_cap = 1;
#else
static const size_t _start_cap = 32;
#endif

void hg_string_init() {
    _string_table.arr = hg_alloc(sizeof(hg_char) * _start_cap);
    _string_table.len = 0;
    _string_table.cap = _start_cap;
}

void hg_string_free() {
    for (size_t i = 0; i < _string_table.len; i++) {
        hg_free((void*)_string_table.arr[i]);
    }
    hg_free(_string_table.arr);
}

static void _string_table_append(hg_char s) {
    if (_string_table.len >= _string_table.cap) {
        _string_table.cap *= 2;
        _string_table.arr =
            hg_realloc(_string_table.arr, sizeof(hg_char) * _string_table.cap);
    }

    _string_table.arr[_string_table.len++] = s;
}

hg_char hg_string_new(const char* from, size_t len) {
    char* s = hg_alloc(sizeof(char) * (len + 1));
    s[len]  = '\0';
    _string_table_append(s);

    return strncpy(s, from, len);
}
