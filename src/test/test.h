#include <stdio.h>
#include <time.h>

#define test_assert(e)                                                                                               \
    do {                                                                                                             \
        (e) ? 1 : fprintf(stderr, "test: %s:%d: in %s: Assertion `%s' failed.\n", __FILE__, __LINE__, __func__, #e); \
    } while (0)

#define test_func(fn) static void test_##fn()

#define test_main(...)                \
    int main(int argc, char** argv) { \
        ((void)argc);                 \
        ((void)argv);                 \
        __VA_ARGS__;                  \
        return 0;                     \
    }

#define printf_dec_format(x) \
    _Generic((x), \
    char: "%c", \
    signed char: "%hhd", \
    unsigned char: "%hhu", \
    signed short: "%hd", \
    unsigned short: "%hu", \
    signed int: "%d", \
    unsigned int: "%u", \
    long int: "%ld", \
    unsigned long int: "%lu", \
    long long int: "%lld", \
    unsigned long long int: "%llu", \
    float: "%f", \
    double: "%f", \
    long double: "%Lf", \
    char *: "%s", \
    void *: "%p")

#define print(x)               printf(printf_dec_format(x), x)
#define println(x)             printf(printf_dec_format(x), x), printf("\n")
#define test_println(fmt, ...) printf("TEST: " fmt "\n", __VA_ARGS__)

typedef clock_t duration_t;

static clock_t now() {
    return clock();
}

static duration_t since(clock_t t) {
    return clock() - t;
}

const static long long int SEC      = CLOCKS_PER_SEC;
const static long long int MILLISEC = SEC / 1000;
const static long long int MACROSEC = MILLISEC / 1000;

static const char* duration2str(duration_t d, long long int type) {
    char* s;
    switch (type) {
    case SEC:
        s = "seconds";
        break;
    case MILLISEC:
        s = "milliseconds";
        break;
    case MACROSEC:
        s = "macrosec";
        break;
    }

    static _Thread_local char buf[1024];
    sprintf(buf, "%lld %s", (long long int)(d) / type, s);
    return buf;
}
