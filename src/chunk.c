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
    size_t capacity = array_grow_(chk->capacity, chk->len);

    if (chk->capacity != capacity) {
        chk->code = array_realloc_(chk->code, uint8_t, chk->capacity, capacity);
        chk->capacity = capacity;
    }
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
    return value_array_push(&chk->statics, value);
}

int chunk_dump(struct chunk* chk, FILE* fp) {
    unimplemented_("TODO");
}
struct chunk* chunk_load(FILE* fp) {
    unimplemented_("TODO");
}

void chunk_disassemble(struct chunk* chk) {
#define print_(s) printf("0x%04lx %s\n", i, s)
#define read_word_()
    for (size_t i = 0; i < chk->len; i++) {
        switch (chk->code[i]) {
        case OP_NOP:
            print_("nop");
            break;
        case OP_STATIC: {
            print_("static");
            printf("%7s", "");
            uint16_t t = (uint16_t)chk->code[i + 1] << 8 | chk->code[i + 2];
            i += 2;
            hg_value_write(chk->statics.values[t], stdout);
            printf("\n");
        } break;
        case OP_NIL:
            print_("nil");
            break;
        case OP_TRUE:
            print_("true");
            break;
        case OP_FALSE:
            print_("false");
            break;
        case OP_EQUAL:
            print_("equal");
            break;
        case OP_GREATER:
            print_("greater");
            break;
        case OP_LESS:
            print_("less");
            break;
        case OP_GREATER_EQUAL:
            print_("greater equal");
            break;
        case OP_LESS_EQUAL:
            print_("less equal");
            break;
        case OP_ADD:
            print_("add");
            break;
        case OP_SUBTRACT:
            print_("sub");
            break;
        case OP_MULTIPLY:
            print_("mul");
            break;
        case OP_DIVIDE:
            print_("div");
            break;
        case OP_MODULO:
            print_("mod");
            break;
        case OP_NOT:
            print_("not");
            break;
        case OP_NEGATE:
            print_("negate");
            break;
        case OP_POP:
            print_("pop");
            break;
        case OP_GET_LOCAL:
            break;
        case OP_SET_LOCAL:
            break;
        case OP_JUMP:
            break;
        case OP_JUMP_IF_FALSE:
            break;
        case OP_JUMP_BACK:
            break;
        case OP_CALL:
            break;
        default:
            unimplemented_("type :%x", chk->code[i]);
        }
    }
#undef print_
}
