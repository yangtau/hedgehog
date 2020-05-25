#ifndef _HG_COMMON_H_
#define _HG_COMMON_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define unimplemented_(...)                                         \
    do {                                                            \
        fprintf(stderr, "unimplemented in function %s:\n", __func__); \
        fprintf(stderr, __VA_ARGS__);                               \
        exit(-1);                                                   \
    } while (0)

#define unreachable_()                                                  \
    do {                                                                \
        fprintf(stderr, "reach unreachable in function %s\n", __func__); \
        exit(-1);                                                       \
    } while (0)

#endif // _HG_COMMON_H_
