#ifndef _HG_COMPILE_H_
#define _HG_COMPILE_H_
#include "ast_node.h"
#include "chunk.h"
#include "hash.h"

struct compiler_context {
    // <var: string, loc: int>
    // var: varialbe name
    // loc: location of the varialbe in the chunk->statics
    struct hash_map global_vars;
    struct hash_map local_vars;
    int scope_depth; // depth 0 for global vars
};
static _force_inline_ void compiler_context_init(struct compiler_context* ctx) {
    hash_map_init(&ctx->global_vars);
    ctx->scope_depth = 0;
}
static _force_inline_ void compiler_context_free(struct compiler_context* ctx) {
    hash_map_free(&ctx->global_vars);
}

/* compile: compile AST into chunk
 * @RETURN: 0 if success
 */
int compile(struct compiler_context* ctx, struct ast_node* node,
            struct chunk* chunk);

#endif // _HG_COMPILE_H_
