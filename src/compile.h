#ifndef _HG_COMPILE_H_
#define _HG_COMPILE_H_
#include "ast_node.h"
#include "chunk.h"
#include "hash.h"

struct compile_state {
    struct hash_map globa_vars; // <string, int>
};
/* compile: compile AST into chunk
 * @RETURN: 0 if success
 */
int compile(struct ast_node* node, struct chunk* chunk);

#endif // _HG_COMPILE_H_
