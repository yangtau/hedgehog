#ifndef HG_COMMON_H_
#define HG_COMMON_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <execinfo.h>
#include <unistd.h>

#define struct_field_offset_(st, field) ((intptr_t)(&((st*)0)->field))

#define _force_inline_ inline __attribute__((always_inline))

static _force_inline_ void print_backtrace() {
    void* callstack[128];
    size_t frames = backtrace(callstack, 128);
    backtrace_symbols_fd(callstack, frames, STDERR_FILENO);
}

#define unimplemented_(...)                                           \
    do {                                                              \
        fprintf(stderr, "unimplemented in function %s:\n", __func__); \
        fprintf(stderr, __VA_ARGS__);                                 \
        fprintf(stderr, "\n");                                        \
        print_backtrace();                                            \
        exit(-1);                                                     \
    } while (0)

#define unreachable_()                                             \
    do {                                                           \
        fprintf(stderr, "unreachable in function %s\n", __func__); \
        print_backtrace();                                         \
        exit(-1);                                                  \
    } while (0)

#define error_(...)                                           \
    do {                                                      \
        fprintf(stderr, "error in function %s:\n", __func__); \
        fprintf(stderr, __VA_ARGS__);                         \
        fprintf(stderr, "\n");                                \
        print_backtrace();                                    \
        exit(-1);                                             \
    } while (0)

// type checks:
#define is_compatible_(x, T) _Generic((x), T : true, default : false)
#define ensure_type_(p, T)   _Static_assert(is_compatible_(p, T), "incorrect type for parameter '" #p "', expected " #T)

// types used by inner HG
typedef int64_t hg_int;
typedef double hg_float;
typedef const char* hg_str;
typedef bool hg_bool;

#define HGFormatInt   PRId64
#define HGFormatFloat "lf"

// disable unused warnings
#define unused_(x) ((void)(x))

#endif // HG_COMMON_H_
