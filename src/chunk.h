#ifndef _HG_CHUNK_H_
#define _HG_CHUNK_H_
#include "common.h"
#include "value.h"

enum opcode {
    OP_NOP,
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
    OP_POP, // pop a byte
    OP_GET_CONST,
    OP_SET_STATIC,
    OP_GET_STATIC,
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
    // TODO: isolate static area from chunk
    //       `statics` should not be in one function, it should be shared globally.
    struct value_array statics; // global variables area
    struct value_array consts;  // constants area
    uint8_t* code;
};

void chunk_init(struct chunk* chk);
void chunk_free(struct chunk* chk);

int chunk_write(struct chunk* chk, uint8_t byte);
static _force_inline_ int chunk_write_word(struct chunk* chk, uint16_t word);
void chunk_patch_word(struct chunk* chk, uint16_t word, int pos);
uint16_t chunk_add_static(struct chunk* chk, struct hg_value value);
uint16_t chunk_add_const(struct chunk* chk, struct hg_value value);
int chunk_dump(struct chunk* chk, FILE* fp);
struct chunk* chunk_load(FILE* fp);

int chunk_disassemble_ins(struct chunk* chk, int i);
void chunk_disassemble(struct chunk* chk);

static _force_inline_ int chunk_write_word(struct chunk* chk, uint16_t word) {
    int r = chunk_write(chk, (word >> 8) & 0xff);
    chunk_write(chk, word & 0xff);
    return r;
}
#endif // _HG_CHUNK_H_
