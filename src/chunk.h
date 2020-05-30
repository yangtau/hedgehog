#ifndef _HG_CHUNK_H_
#define _HG_CHUNK_H_
#include "common.h"
#include "value.h"

enum opcode {
    OP_NOP,
    OP_STATIC,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_GREATER_EQUAL,
    OP_LESS_EQUAL,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MODULO,
    OP_NOT,
    OP_NEGATE,
    OP_POP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_JUMP_BACK,
    OP_CALL,
};

#define CHUNK_INIT_CAPACITY (256u)
struct chunk {
    size_t len;
    size_t capacity;
    struct value_array statics;
    uint8_t* code;
};

void chunk_init(struct chunk* chk);
void chunk_free(struct chunk* chk);

void chunk_write(struct chunk* chk, uint8_t byte);
uint16_t chunk_add_static(struct chunk* chk, struct hg_value value);
#define chunk_write_word_(chk, word)             \
    do {                                         \
        struct chunk* _chk = (chk);              \
        uint16_t _word     = (word);             \
        chunk_write(_chk, (_word >> 8) && 0xff); \
        chunk_write(_chk, _word && 0xff);        \
    } while (0)

int chunk_dump(struct chunk* chk, FILE* fp);
struct chunk* chunk_load(FILE* fp);
void chunk_disassable(struct chunk* chk);
#endif // _HG_CHUNK_H_
