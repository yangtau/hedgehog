#ifndef _HG_CHUNK_H_
#define _HG_CHUNK_H_
#include "common.h"
#include "value.h"

//> opcode
enum opcode {
    OP_QUIT, // normally quit the VM
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
    OP_POP,
    OP_GET_CONST,
    OP_SET_STATIC,
    OP_GET_STATIC,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_JUMP_BACK,
    OP_CALL,
    OP_RET,
};
//< opcode

//> hg_function
struct hg_function {
    int argc;      // argument count
    uint8_t* addr; // function entry address
    struct hg_value name;
};

//< hg_function

//> chunk
#define CHUNK_INIT_CAPACITY (256u)
struct chunk {
    size_t len;
    size_t capacity;
    struct value_array statics; // global variables area
    struct value_array consts;  // constants area
    struct {
        size_t len;
        size_t capacity;
        struct hg_function* funcs;
    } funcs;
    uint8_t* code;
};

typedef struct chunk* shared_chunk_p;

void chunk_init(struct chunk* chk);
void chunk_free(struct chunk* chk);

int chunk_write(struct chunk* chk, uint8_t byte);
static _force_inline_ int chunk_write_word(struct chunk* chk, uint16_t word);
void chunk_patch_word(struct chunk* chk, uint16_t word, int pos);
uint16_t chunk_add_static(struct chunk* chk, struct hg_value value);
uint16_t chunk_add_const(struct chunk* chk, struct hg_value value);
uint16_t chunk_add_func(struct chunk* chk, struct hg_function func);

int chunk_dump(struct chunk* chk, FILE* fp);
struct chunk* chunk_load(FILE* fp);

int chunk_disassemble_ins(struct chunk* chk, int i);
void chunk_disassemble(struct chunk* chk);

static _force_inline_ int chunk_write_word(struct chunk* chk, uint16_t word) {
    int r = chunk_write(chk, (word >> 8) & 0xff);
    chunk_write(chk, word & 0xff);
    return r;
}
//< chunk
#endif // _HG_CHUNK_H_
