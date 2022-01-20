#include "test.h"

#include "../string.h"
#include <string.h>

test_func(string_buffer) {
    hg_string_init();

    struct hg_string_buffer buf;

    {
        //  empty string
        hg_string_buffer_init(&buf, 10);

        test_assert(buf.len == 0);
        test_assert(buf._cap == 10);
        test_assert(!buf._moved);

        const char* t = hg_string_buffer_to_str(&buf);
        test_assert(strcmp("", t) == 0);
        test_assert(buf._moved);

        hg_string_buffer_free(&buf);
    }

    {
        hg_string_buffer_init(&buf, 10);
        test_assert(buf.len == 0);

        test_assert(hg_string_buffer_append(&buf, "hello, %s", "world"));
        println(buf.len);
        test_assert(buf.len == strlen("hello, world"));

        test_assert(hg_string_buffer_append(&buf, "hello, %s", "world"));
        test_assert(buf.len == strlen("hello, worldhello, world"));

        test_assert(hg_string_buffer_append(&buf, "hello, %s", "world"));
        test_assert(buf.len == strlen("hello, worldhello, worldhello, world"));

        test_assert(
            hg_string_buffer_append(&buf, " a=%d+%d,b=%c", 10, 20, 'c'));
        test_assert(buf.len ==
                    strlen("hello, worldhello, worldhello, world a=10+20,b=c"));

        test_assert(hg_string_buffer_append(&buf, ""));
        test_assert(buf.len ==
                    strlen("hello, worldhello, worldhello, world a=10+20,b=c"));

        test_assert(hg_string_buffer_append(&buf, "  "));
        test_assert(
            buf.len ==
            strlen("hello, worldhello, worldhello, world a=10+20,b=c  "));

        const char* t = hg_string_buffer_to_str(&buf);
        test_assert(strcmp("hello, worldhello, worldhello, world a=10+20,b=c  ",
                           t) == 0);
        test_assert(buf._moved);

        hg_string_buffer_free(&buf);
    }

    hg_string_free();
}

test_main(test_string_buffer();)
