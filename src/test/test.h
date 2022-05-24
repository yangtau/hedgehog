#include <stdio.h>
#include <time.h>

#define test_assert(e)                                                                                               \
    do {                                                                                                             \
        (e) ? 1 : fprintf(stderr, "test: %s:%d: in %s: Assertion `%s' failed.\n", __FILE__, __LINE__, __func__, #e); \
    } while (0)

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
#define test_println(fmt, ...) printf("        TEST: " fmt "\n", __VA_ARGS__)

typedef clock_t duration_t;

static inline clock_t now() {
    return clock();
}

static inline duration_t since(clock_t t) {
    return clock() - t;
}

#define SEC      ((long long int)CLOCKS_PER_SEC)
#define MILLISEC ((long long int)SEC / 1000)
#define MACROSEC ((long long int)MILLISEC / 1000)

static inline const char* duration2str(duration_t d, long long int type) {
    const char* s;
    if (type == SEC)
        s = "seconds";
    if (type == MILLISEC)
        s = "milliseconds";
    if (type == MACROSEC)
        s = "macroseconds";

    static _Thread_local char buf[1024];
    sprintf(buf, "%lld %s", (long long int)(d) / type, s);
    return buf;
}

#define test_main(...)                                           \
    int main(int argc, char** argv) {                            \
        ((void)argc);                                            \
        ((void)argv);                                            \
        concat(run_test_func_, nargs(__VA_ARGS__))(__VA_ARGS__); \
        println("");                                             \
        return 0;                                                \
    }

#define _concat(x, y) x##y
#define concat(x, y)  _concat(x, y)

#define _nargs(_100, _99, _98, _97, _96, _95, _94, _93, _92, _91, _90, _89, _88, _87, _86, _85, _84, _83, _82, _81, \
               _80, _79, _78, _77, _76, _75, _74, _73, _72, _71, _70, _69, _68, _67, _66, _65, _64, _63, _62, _61,  \
               _60, _59, _58, _57, _56, _55, _54, _53, _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41,  \
               _40, _39, _38, _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, _22, _21,  \
               _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...)   \
    N

#define nargs(...)                                                                                                     \
    _nargs(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78,   \
           77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, \
           50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, \
           23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define run_test_func_1(f)        \
    println("    run " #f "..."); \
    f();

#define run_test_func_2(f, ...) \
    run_test_func_1(f);         \
    run_test_func_1(__VA_ARGS__);
#define run_test_func_3(f, ...) \
    run_test_func_1(f);         \
    run_test_func_2(__VA_ARGS__);
#define run_test_func_4(f, ...) \
    run_test_func_1(f);         \
    run_test_func_3(__VA_ARGS__);
#define run_test_func_5(f, ...) \
    run_test_func_1(f);         \
    run_test_func_4(__VA_ARGS__);
#define run_test_func_6(f, ...) \
    run_test_func_1(f);         \
    run_test_func_5(__VA_ARGS__);
#define run_test_func_7(f, ...) \
    run_test_func_1(f);         \
    run_test_func_6(__VA_ARGS__);
#define run_test_func_8(f, ...) \
    run_test_func_1(f);         \
    run_test_func_7(__VA_ARGS__);
#define run_test_func_9(f, ...) \
    run_test_func_1(f);         \
    run_test_func_8(__VA_ARGS__);
#define run_test_func_10(f, ...) \
    run_test_func_1(f);          \
    run_test_func_9(__VA_ARGS__);
#define run_test_func_11(f, ...) \
    run_test_func_1(f);          \
    run_test_func_10(__VA_ARGS__);
#define run_test_func_12(f, ...) \
    run_test_func_1(f);          \
    run_test_func_11(__VA_ARGS__);
#define run_test_func_13(f, ...) \
    run_test_func_1(f);          \
    run_test_func_12(__VA_ARGS__);
#define run_test_func_14(f, ...) \
    run_test_func_1(f);          \
    run_test_func_13(__VA_ARGS__);
#define run_test_func_15(f, ...) \
    run_test_func_1(f);          \
    run_test_func_14(__VA_ARGS__);
#define run_test_func_16(f, ...) \
    run_test_func_1(f);          \
    run_test_func_15(__VA_ARGS__);
#define run_test_func_17(f, ...) \
    run_test_func_1(f);          \
    run_test_func_16(__VA_ARGS__);
#define run_test_func_18(f, ...) \
    run_test_func_1(f);          \
    run_test_func_17(__VA_ARGS__);
#define run_test_func_19(f, ...) \
    run_test_func_1(f);          \
    run_test_func_18(__VA_ARGS__);
#define run_test_func_20(f, ...) \
    run_test_func_1(f);          \
    run_test_func_19(__VA_ARGS__);
#define run_test_func_21(f, ...) \
    run_test_func_1(f);          \
    run_test_func_20(__VA_ARGS__);
#define run_test_func_22(f, ...) \
    run_test_func_1(f);          \
    run_test_func_21(__VA_ARGS__);
#define run_test_func_23(f, ...) \
    run_test_func_1(f);          \
    run_test_func_22(__VA_ARGS__);
#define run_test_func_24(f, ...) \
    run_test_func_1(f);          \
    run_test_func_23(__VA_ARGS__);
#define run_test_func_25(f, ...) \
    run_test_func_1(f);          \
    run_test_func_24(__VA_ARGS__);
#define run_test_func_26(f, ...) \
    run_test_func_1(f);          \
    run_test_func_25(__VA_ARGS__);
#define run_test_func_27(f, ...) \
    run_test_func_1(f);          \
    run_test_func_26(__VA_ARGS__);
#define run_test_func_28(f, ...) \
    run_test_func_1(f);          \
    run_test_func_27(__VA_ARGS__);
#define run_test_func_29(f, ...) \
    run_test_func_1(f);          \
    run_test_func_28(__VA_ARGS__);
#define run_test_func_30(f, ...) \
    run_test_func_1(f);          \
    run_test_func_29(__VA_ARGS__);
#define run_test_func_31(f, ...) \
    run_test_func_1(f);          \
    run_test_func_30(__VA_ARGS__);
#define run_test_func_32(f, ...) \
    run_test_func_1(f);          \
    run_test_func_31(__VA_ARGS__);

