#include "memory.h"

static size_t memory_usage = 0;

void* hg_realloc(void* ptr, size_t old_size, size_t new_size) {
    assert(memory_usage >= old_size);
    memory_usage -= old_size;
    memory_usage += new_size;

    ptr = realloc(ptr, new_size);

    return ptr;
}

size_t hg_memory_usage() {
    return memory_usage;
}
