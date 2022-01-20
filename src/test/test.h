#include <stdio.h>

#define test_assert(e)                                                        \
    do {                                                                      \
        (e) ? 1                                                               \
            : fprintf(stderr, "test: %s:%d: in %s: Assertion `%s' failed.\n", \
                      __FILE__, __LINE__, __func__, #e);                      \
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

#define print(x)   printf(printf_dec_format(x), x)
#define println(x) printf(printf_dec_format(x), x), printf("\n");
