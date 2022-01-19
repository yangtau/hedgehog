#include "ast.h"
#include "memory.h"

#include <stdarg.h>

struct hg_ast {
    struct hg_ast_node* node;
};

struct hg_ast* hg_ast_new() {
    return NULL;
}

void hg_ast_free(struct hg_ast* ast) {
}
