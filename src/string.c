#include "string.h"
#include "common.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static struct {
    hg_str* arr;
    size_t len;
    size_t cap;
} _string_table;

#ifdef HG_DEBUG
static const size_t _start_cap = 1;
#else
static const size_t _start_cap = 32;
#endif

void hg_strtable_init() {
    _string_table.arr = hg_alloc(sizeof(hg_str) * _start_cap);
    _string_table.len = 0;
    _string_table.cap = _start_cap;
}

void hg_strtable_free() {
    for (size_t i = 0; i < _string_table.len; i++) {
        hg_free((void*)_string_table.arr[i]);
    }
    hg_free(_string_table.arr);
}

static void _strtable_append(hg_str s) {
    if (_string_table.len >= _string_table.cap) {
        _string_table.cap *= 2;
        _string_table.arr =
            hg_realloc(_string_table.arr, sizeof(hg_str) * _string_table.cap);
    }

    _string_table.arr[_string_table.len++] = s;
}

hg_str hg_str_new(const char* from, size_t len) {
    char* s = hg_alloc(sizeof(char) * (len + 1));
    s[len]  = '\0';
    _strtable_append(s);

    return strncpy(s, from, len);
}

void hg_strbuf_init(struct hg_strbuf* buf) {
    assert(_start_cap > 0);
    buf->_cap   = _start_cap;
    buf->len    = 0;
    buf->_moved = false;
    buf->_str   = hg_alloc(sizeof(char) * buf->_cap);
}

void hg_strbuf_free(struct hg_strbuf* buf) {
    if (!buf->_moved) {
        hg_free(buf->_str);
    }
}

static void _strbuf_expend(struct hg_strbuf* buf, size_t want) {
    assert(buf->_cap > buf->len);
    while ((buf->_cap - buf->len - 1) < want)
        buf->_cap *= 2;

    buf->_str = hg_realloc(buf->_str, sizeof(char) * buf->_cap);
}

bool hg_strbuf_append(struct hg_strbuf* buf, const char* fmt, ...) {
    va_list args;
    assert(buf->_cap > buf->len);

    for (int i = 0; i < 2; i++) {
        size_t left = buf->_cap - buf->len;
        char* start = buf->_str + buf->len;

        va_start(args, fmt);
        int written = vsnprintf(start, left, fmt, args);
        va_end(args);

        if (written < 0) {
            return false; // error
        }
        if (written < (int)(left)) {
            buf->len += written;
            return true;
        }

        if (i == 0) {
            // buf is not enough, retry
            // only extend buffer at the first try
            _strbuf_expend(buf, written);
        }
    }

    assert(0);
    return false;
}

hg_str hg_strbuf_to_str(struct hg_strbuf* buf) {
    assert(buf->_cap > buf->len);

    buf->_str[buf->len] = '\0';
    buf->_moved         = true;

    _strtable_append(buf->_str);
    return buf->_str;
}
