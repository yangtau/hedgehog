#include "compile.h"
#include "ast_node.h"
#include "common.h"

static int compile_ast_node_op(void* _node, struct chunk* chk,
                               struct compile_state* state) {
#define compile_left_right()              \
    do {                                  \
        compile(node->left, chk, state);  \
        compile(node->right, chk, state); \
    } while (0)
    struct ast_node_op* node = _node;
    switch (node->op) {
    case AST_NODE_OP_AND:
        break;
    case AST_NODE_OP_OR:
        break;
    case AST_NODE_OP_NOT:
        compile(node->right, chk, state);
        chunk_write(chk, OP_NOT);
        break;
    case AST_NODE_OP_NEQ:
        compile_left_right();
        chunk_write(chk, OP_EQUAL);
        chunk_write(chk, OP_NOT);
        break;
    case AST_NODE_OP_EQ:
        compile_left_right();
        chunk_write(chk, OP_EQUAL);
        break;
    case AST_NODE_OP_GE:
        compile_left_right();
        chunk_write(chk, OP_GREATER_EQUAL);
        break;
    case AST_NODE_OP_LE:
        compile_left_right();
        chunk_write(chk, OP_LESS_EQUAL);
        break;
    case AST_NODE_OP_GT:
        compile_left_right();
        chunk_write(chk, OP_GREATER);
        break;
    case AST_NODE_OP_LS:
        compile_left_right();
        chunk_write(chk, OP_LESS);
        break;
    case AST_NODE_OP_NEG:
        compile(node->right, chk, state);
        chunk_write(chk, OP_NEGATE);
        break;
    case AST_NODE_OP_ADD:
        compile_left_right();
        chunk_write(chk, OP_ADD);
        break;
    case AST_NODE_OP_SUB:
        compile_left_right();
        chunk_write(chk, OP_SUBTRACT);
        break;
    case AST_NODE_OP_MUL:
        compile_left_right();
        chunk_write(chk, OP_MULTIPLY);
        break;
    case AST_NODE_OP_DIV:
        compile_left_right();
        chunk_write(chk, OP_DIVIDE);
        break;
    case AST_NODE_OP_MOD:
        compile_left_right();
        chunk_write(chk, OP_MODULO);
        break;
    case AST_NODE_OP_DDOT:
        unimplemented_("TODO: operator `..`");
        break;
    default:
        unimplemented_("operator type %x", node->op);
    }
    return 0;
#undef compile_left_right
}

static int compile_ast_node_value(void* _value, struct chunk* chk, struct compile_state* state) {
    struct hg_value* val = _value;

    if (VAL_IS_BOOL(*val)) {
        chunk_write(chk, VAL_AS_BOOL(*val) ? OP_TRUE : OP_FALSE);
    } else {
        uint16_t loc = chunk_add_static(chk, *val);
        chunk_write(chk, OP_STATIC);
        chunk_write_word(chk, loc);
    }
    return 0;
}

static int compile_ast_node_stats(void* _stats, struct chunk* chk,
                                  struct compile_state* state) {
    struct ast_node_array* stats = _stats;

    int rc = 0;
    for (size_t i = 0; i < stats->len; i++) {
        if ((rc = compile(stats->arr[i], chk, state)) != 0) {
            break;
        }
    }
    return rc;
}

static int compile_ast_node_tuple(void* _tuple, struct chunk* chk,
                                  struct compile_state* state) {
    struct ast_node_array* tuple = _tuple;
    switch (tuple->len) {
    case 0: // nil
        chunk_write(chk, OP_NIL);
        break;
    case 1: // unpack if there is only one element in the tuple
        compile(tuple->arr[0], chk, state);
        break;
    default:
        unimplemented_("TODO: tuple with length of %ld", tuple->len);
    }
    return 0;
}

// compile_funcs is a static array of const pointer to function
static int (*const compile_funcs[])(void*, struct chunk*,
                                    struct compile_state*) = {
    [AST_NODE_OP]       = compile_ast_node_op,
    [AST_NODE_VALUE]    = compile_ast_node_value, // node->node = hg_value
    [AST_NODE_STATS]    = compile_ast_node_stats,
    [AST_NODE_TUPLE]    = compile_ast_node_tuple,
    [AST_NODE_ASSIGN]   = NULL,
    [AST_NODE_IF]       = NULL,
    [AST_NODE_FOR]      = NULL,
    [AST_NODE_CALL]     = NULL,
    [AST_NODE_FUNC]     = NULL,
    [AST_NODE_WHILE]    = NULL,
    [AST_NODE_BREAK]    = NULL, // node->node = NULL
    [AST_NODE_CONTINUE] = NULL, // node->node = NULL
    [AST_NODE_RETURN]   = NULL, // node->node = expr
    [AST_NODE_VARS]     = NULL,
    [AST_NODE_ARGS]     = NULL,
    [AST_NODE_LIST]     = NULL,
};

int compile(struct ast_node* node, struct chunk* chunk,
            struct compile_state* state) {
    return compile_funcs[node->type](node->node, chunk, state);
}
