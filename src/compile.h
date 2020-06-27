#ifndef _HG_COMPILE_H_
#define _HG_COMPILE_H_
#include "ast_node.h"
#include "chunk.h"
#include "hash.h"
#include "common.h"

//> compiler_context
struct compiler_context {
    // global_vars: <var: string, loc: int>
    // var: varialbe name, loc: location of the varialbe in the chunk->statics
    struct hash_map global_vars;
    struct frame_context* frame;
    int scope_depth; // depth 0 for global vars
    struct local* local_list;

    struct hash_map funcs; // TODO: remove this after adding closure
    shared_chunk_p chk;
};
void compiler_context_init(struct compiler_context* ctx, shared_chunk_p chk);
void compiler_context_free(struct compiler_context* ctx);
//< compiler_context

/* compile: compile AST into chunk
 * @RETURN: 0 if success
 */
int compile(struct compiler_context* ctx, struct ast_node* node);

#endif // _HG_COMPILE_H_
