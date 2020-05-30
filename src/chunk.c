#include "chunk.h"
#include "memory.h"

void chunk_init(struct chunk* chk) {
    chk->code     = array_alloc_(uint8_t, CHUNK_INIT_CAPACITY);
    chk->capacity = CHUNK_INIT_CAPACITY;
    chk->len      = 0;

    value_array_init(&chk->statics);
}

void chunk_free(struct chunk* chk) {
    value_array_free(&chk->statics);
    array_free_(chk->code, uint8_t, chk->capacity);

    chk->capacity = 0;
    chk->len      = 0;
    chk->code     = NULL;
}

static inline void chunk_resize(struct chunk* chk) {
    size_t capacity = array_new_size_(chk->capacity, chk->len);

    if (chk->capacity != capacity) {
        chk->code = array_realloc_(chk->code, uint8_t, chk->capacity, capacity);
    }
    chk->capacity = capacity;
}

void chunk_write(struct chunk* chk, uint8_t byte) {
    chunk_resize(chk);
    chk->code[chk->len++] = byte;
}

uint16_t chunk_add_static(struct chunk* chk, struct hg_value value) {
    if (chk->statics.len >= UINT16_MAX) {
        error_("The maximum number of constants of a chunk is %u",
               UINT16_MAX + 1u);
    }
    return value_array_push_(&chk->statics, value);
}

int chunk_dump(struct chunk* chk, FILE* fp) {
    unimplemented_("TODO");
}
struct chunk* chunk_load(FILE* fp) {
    unimplemented_("TODO");
}
void chunk_disassable(struct chunk* chk) {
    unimplemented_("TODO");
}
