#include "memory.h"

static size_t memory_useage = 0;

void* hg_realloc(void* ptr, size_t old_size, size_t new_size) {
   memory_useage += new_size; 
   assert(memory_useage >= old_size);
   memory_useage -= old_size;
   return realloc(ptr, new_size);
}
