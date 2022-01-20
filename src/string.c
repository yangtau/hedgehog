#include "string.h"
#include "memory.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

static struct {
    hg_char* arr;
    size_t len;
    size_t cap;
} _string_table;

#ifdef HG_DEBUG
static const size_t _start_cap              = 1;
static const size_t _buffer_shrink_threshold = 0;
#else
static const size_t _start_cap              = 32;
static const size_t _buffer_shrink_threshold = 32;
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

void hg_string_buffer_init(struct hg_string_buffer* buf, size_t cap) {
    assert(cap > 1);

    buf->_cap   = cap;
    buf->len    = 0;
    buf->_moved = false;
    buf->_str   = hg_alloc(sizeof(char) * cap);
}

void hg_string_buffer_free(struct hg_string_buffer* buf) {
    if (!buf->_moved) {
        hg_free(buf->_str);
    }
}

static void _string_buffer_expend(struct hg_string_buffer* buf, size_t want) {

    while ((buf->_cap - buf->len - 1) < want) {
        buf->_cap *= 2;
    }

    buf->_str = hg_realloc(buf->_str, sizeof(char) * buf->_cap);
}

static void _string_buffer_shrink(struct hg_string_buffer* buf) {
    assert(buf->_cap > buf->len + 1);
    buf->_str = hg_realloc(buf->_str, sizeof(char) * (buf->len + 1));
}

bool hg_string_buffer_append(struct hg_string_buffer* buf, const char* fmt,
                             ...) {
    va_list args;
    assert(buf->_cap > buf->len + 1);

    for (int i = 0; i < 2; i++) {
        size_t left = buf->_cap - buf->len - 1; // reserve 1 char for '\0'
        char* start = buf->_str + buf->len;

        va_start(args, fmt);
        int written = vsnprintf(start, left, fmt, args);
        va_end(args);

        if (written < 0) {
            return false; // error
        }
        if (written <= (int)(left)) {
            buf->len += written;
            return true;
        }

        if (i == 0) {
            // buf is not enough, retry
            _string_buffer_expend(buf, written);
        }
    }
    return false;
}

hg_char hg_string_buffer_to_str(struct hg_string_buffer* buf) {
    if (buf->_cap > buf->len + 1 + _buffer_shrink_threshold) {
        _string_buffer_shrink(buf);
    }

    buf->_str[buf->len] = '\0';
    buf->_moved         = true;

    _string_table_append(buf->_str);
    return buf->_str;
}
