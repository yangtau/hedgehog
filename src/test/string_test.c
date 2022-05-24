#include "test.h"

#include "../string.h"
#include <string.h>

void test_strbuf() {
    hg_strtable_init();

    struct hg_strbuf buf;

    {
        //  empty string
        hg_strbuf_init(&buf);

        test_assert(buf.len == 0);
        test_assert(!buf._moved);

        const char* t = hg_strbuf_to_str(&buf);
        test_assert(strcmp("", t) == 0);
        test_assert(buf._moved);

        hg_strbuf_free(&buf);
    }

    {
        hg_strbuf_init(&buf);
#define spaces "        "
        hg_strbuf_append(&buf, spaces);
        hg_strbuf_append(&buf, spaces);
        hg_strbuf_append(&buf, spaces);
        hg_strbuf_append(&buf, spaces);

        test_assert(buf.len == 4 * strlen(spaces));
        const char* t = hg_strbuf_to_str(&buf);
        test_assert(strcmp(spaces spaces spaces spaces, t) == 0);

#undef spaces
        hg_strbuf_free(&buf);
    }

    {
        hg_strbuf_init(&buf);
        test_assert(buf.len == 0);

        test_assert(hg_strbuf_append(&buf, "hello, %s", "world"));
        test_assert(buf.len == strlen("hello, world"));

        test_assert(hg_strbuf_append(&buf, "hello, %s", "world"));
        test_assert(buf.len == strlen("hello, worldhello, world"));

        test_assert(hg_strbuf_append(&buf, "hello, %s", "world"));
        test_assert(buf.len == strlen("hello, worldhello, worldhello, world"));

        test_assert(hg_strbuf_append(&buf, " a=%d+%d,b=%c", 10, 20, 'c'));
        test_assert(buf.len == strlen("hello, worldhello, worldhello, world a=10+20,b=c"));

        test_assert(hg_strbuf_append(&buf, ""));
        test_assert(buf.len == strlen("hello, worldhello, worldhello, world a=10+20,b=c"));

        test_assert(hg_strbuf_append(&buf, "  "));
        test_assert(buf.len == strlen("hello, worldhello, worldhello, world a=10+20,b=c  "));

        const char* t = hg_strbuf_to_str(&buf);
        test_assert(strcmp("hello, worldhello, worldhello, world a=10+20,b=c  ", t) == 0);
        test_assert(buf._moved);

        hg_strbuf_free(&buf);
    }

    hg_strtable_free();
}

test_main(test_strbuf)
