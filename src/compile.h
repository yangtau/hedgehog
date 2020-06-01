#ifndef _HG_COMPILE_H_
#define _HG_COMPILE_H_
#include "ast_node.h"
#include "chunk.h"
#include "hash.h"

struct compile_state {
    // <var: string, loc: int>
    // var: varialbe name
    // loc: location of the varialbe in the chunk->statics
    struct hash_map globa_vars;
    struct hash_map local_vars;
};
static _force_inline_ void compile_state_init(struct compile_state* state) {
    hash_map_init(&state->globa_vars);
}
static _force_inline_ void compile_state_free(struct compile_state* state) {
    hash_map_free(&state->globa_vars);
}

/* compile: compile AST into chunk
 * @RETURN: 0 if success
 */
int compile(struct ast_node* node, struct chunk* chunk,
            struct compile_state* state);

#endif // _HG_COMPILE_H_
