#include "chunk.h"
#include "memory.h"

void chunk_init(struct chunk* chk) {
    chk->code     = array_alloc_(uint8_t, CHUNK_INIT_CAPACITY);
    chk->capacity = CHUNK_INIT_CAPACITY;
    chk->len      = 0;

    value_array_init(&chk->statics);
    value_array_init(&chk->consts);
}

void chunk_free(struct chunk* chk) {
    value_array_free(&chk->statics);
    value_array_free(&chk->consts);
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
        error_("The maximum number of statics of a chunk is %u",
               UINT16_MAX + 1u);
    }
    return value_array_push(&chk->statics, value);
}

uint16_t chunk_add_const(struct chunk* chk, struct hg_value value) {
    if (chk->consts.len >= UINT16_MAX) {
        error_("The maximum number of constants of a chunk is %u",
               UINT16_MAX + 1u);
    }
    return value_array_push(&chk->consts, value);
}

int chunk_dump(struct chunk* chk, FILE* fp) {
    unimplemented_("TODO");
}
struct chunk* chunk_load(FILE* fp) {
    unimplemented_("TODO");
}

void chunk_disassemble(struct chunk* chk) {
#define print_(s) printf("0x%04lx %s", i, (s))
#define read_word_()
    for (size_t i = 0; i < chk->len; i++) {
        switch (chk->code[i]) {
        case OP_NOP:
            print_("nop\n");
            break;
        case OP_GET_CONST: {
            print_("get ");
            uint16_t t = (uint16_t)chk->code[i + 1] << 8 | chk->code[i + 2];
            i += 2;
            printf("consts[%hu]=", t);
            hg_value_write(chk->consts.values[t], stdout);
            printf("\n");

        } break;
        case OP_GET_STATIC: {
            print_("get ");
            uint16_t t = (uint16_t)chk->code[i + 1] << 8 | chk->code[i + 2];
            i += 2;
            printf("statics[%hu]", t);
            printf("\n");
        } break;
        case OP_SET_STATIC: {
            print_("set ");
            uint16_t t = (uint16_t)chk->code[i + 1] << 8 | chk->code[i + 2];
            i += 2;
            printf("statics[%hu]\n", t);
        } break;
        case OP_NIL:
            print_("nil\n");
            break;
        case OP_TRUE:
            print_("true\n");
            break;
        case OP_FALSE:
            print_("false\n");
            break;
        case OP_EQUAL:
            print_("equal\n");
            break;
        case OP_GREATER:
            print_("greater\n");
            break;
        case OP_LESS:
            print_("less\n");
            break;
        case OP_GREATER_EQUAL:
            print_("greater_equal\n");
            break;
        case OP_LESS_EQUAL:
            print_("less_equal\n");
            break;
        case OP_ADD:
            print_("add\n");
            break;
        case OP_SUBTRACT:
            print_("sub\n");
            break;
        case OP_MULTIPLY:
            print_("mul\n");
            break;
        case OP_DIVIDE:
            print_("div\n");
            break;
        case OP_MODULO:
            print_("mod\n");
            break;
        case OP_NOT:
            print_("not\n");
            break;
        case OP_NEGATE:
            print_("negate\n");
            break;
        case OP_POP:
            print_("pop\n");
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
            unimplemented_("type: %x", chk->code[i]);
        }
    }
#undef print_
}
