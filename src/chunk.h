#ifndef _HG_CHUNK_H_
#define _HG_CHUNK_H_
#include "common.h"
#include "value.h"
#define VALUE_ARRAY_MAX_SIZE (UINT16_MAX + 1)

enum opcode {
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_POP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_GET_SUPER,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_CALL,
    OP_INVOKE,
    OP_SUPER_INVOKE,
    OP_CLOSURE,
    OP_CLOSE_UPVALUE,
    OP_RETURN,
    OP_CLASS,
    OP_INHERIT,
    OP_METHOD
};

struct chunk {
    size_t len;
    size_t capacity;
    struct value_array* arr;
    uint8_t code[];
};

struct chunk* chunk_new();
struct chunk* chunk_resize(struct chunk* chk, size_t new_len);
void chunk_free(struct chunk* chk);
void chunk_write(struct chunk *chk, uint8_t byte);
void chunk_write_word(struct chunk *chk, uint16_t word);

int chunk_dump(struct chunk* chk, FILE* fp);
struct chunk* chunk_load(FILE* fp);

#endif // _HG_CHUNK_H_
