#ifndef HG_MEMORY_H_
#define HG_MEMORY_H_

#include "common.h"

static _force_inline_ void* hg_alloc(size_t size) {
    return malloc(size);
}

static _force_inline_ void* hg_realloc(void* p, size_t size) {
    return realloc(p, size);
}

static _force_inline_ void hg_free(void* p) {
    assert(p);
    return free(p);
}

#endif // HG_MEMORY_H_
