#include "memory.h"

static size_t memory_usage = 0;

void* hg_realloc(void* ptr, size_t old_size, size_t new_size) {
   memory_usage += new_size; 
   assert(memory_usage >= old_size);
   memory_usage -= old_size;
   return realloc(ptr, new_size);
}

size_t hg_memory_usage() {
    return memory_usage;
}
