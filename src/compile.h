#ifndef _HG_COMPILE_H_
#define _HG_COMPILE_H_
#include "ast_node.h"
#include "chunk.h"

/* compile: compile AST into bytecode
 */
int compile(struct ast_node* node, struct chunk* chunk);

#endif // _HG_COMPILE_H_
