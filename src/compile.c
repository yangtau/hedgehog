// TODO: Check the number of arguments while compiling fucntion call
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

static int compile_ast_node_variable(struct hg_value id, struct chunk* chk,
                                     struct compile_state* state) {
    if (state->scope_depth == 0) {
        // global
        struct hg_value val = hash_map_get(&state->global_vars, id);
        uint16_t loc;
        if (VAL_IS_UNDEF(val)) {
            fprintf(stderr, "compile error: reference before definition `");
            hg_value_write(id, stderr);
            fprintf(stderr, "`\n");
            return -1;
        } else {
            loc = (uint16_t)VAL_AS_INT(val);
        }

        chunk_write(chk, OP_GET_STATIC);
        chunk_write_word(chk, loc);

    } else {
        // TODO: Compile local variable (get)
        unimplemented_("local");
    }
    return 0;
}

static int compile_ast_node_value(void* _value, struct chunk* chk,
                                  struct compile_state* state) {
    struct hg_value* val = _value;

    if (VAL_IS_BOOL(*val)) {
        chunk_write(chk, VAL_AS_BOOL(*val) ? OP_TRUE : OP_FALSE);
    } else if (VAL_IS_OBJ(*val) && (VAL_AS_OBJ(*val)->type == HG_OBJ_SYMBOL)) {
        return compile_ast_node_variable(*val, chk, state);
    } else {
        uint16_t loc = chunk_add_const(chk, *val);
        chunk_write(chk, OP_GET_CONST);
        chunk_write_word(chk, loc);
    }
    return 0;
}

static int compile_ast_node_stats(void* _stats, struct chunk* chk,
                                  struct compile_state* state) {
    struct ast_node_array* stats = _stats;

    int rc = 0;
    for (size_t i = 0; i < stats->len; i++) {
        rc = compile(stats->arr[i], chk, state);
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
        // TODO: Compile tuple
        unimplemented_("TODO: tuple with length of %ld", tuple->len);
    }
    return 0;
}

static int compile_ast_node_args(void* _args, struct chunk* chk,
                                 struct compile_state* state) {
    struct ast_node_array* args = _args;

    int rc = 0;
    for (int i = args->len - 1; i >= 0; i--) {
        // push inversely
        rc = compile(args->arr[i], chk, state);
    }
    return rc;
}

static int compile_ast_node_vars(void* _vars, struct chunk* chk,
                                 struct compile_state* state) {
    struct ast_node_array* vars = _vars;

    int rc = 0;

    for (size_t i = 0; i < vars->len; i++) {
        struct hg_value* id = vars->arr[i]->node;
        if (state->scope_depth == 0) {
            // global
            struct hg_value val = hash_map_get(&state->global_vars, *id);
            uint16_t loc;
            if (VAL_IS_UNDEF(val)) {
                loc = chunk_add_static(chk, val);
                hash_map_put(&state->global_vars, *id, VAL_INT(loc));
            } else {
                loc = (uint16_t)VAL_AS_INT(val);
            }

            chunk_write(chk, OP_SET_STATIC);
            chunk_write_word(chk, loc);
        } else {
            // TODO: Compile local Variables (set)
            unimplemented_("local vars");
        }
    }
    return rc;
}

static int compile_ast_node_assign(void* _assign, struct chunk* chk,
                                   struct compile_state* state) {
    struct ast_node_assign* node = _assign;
    compile(node->args, chk, state);
    compile(node->vars, chk, state);
    return 0;
}

// compile_funcs is a static array of const pointer to function
static int (*const compile_funcs[])(void*, struct chunk*,
                                    struct compile_state*) = {
    [AST_NODE_OP]       = compile_ast_node_op,
    [AST_NODE_VALUE]    = compile_ast_node_value, // node->node = hg_value
    [AST_NODE_STATS]    = compile_ast_node_stats,
    [AST_NODE_TUPLE]    = compile_ast_node_tuple,
    [AST_NODE_ASSIGN]   = compile_ast_node_assign,
    [AST_NODE_VARS]     = compile_ast_node_vars,
    [AST_NODE_ARGS]     = compile_ast_node_args,
    [AST_NODE_IF]       = NULL,
    [AST_NODE_FOR]      = NULL,
    [AST_NODE_CALL]     = NULL,
    [AST_NODE_FUNC]     = NULL,
    [AST_NODE_WHILE]    = NULL,
    [AST_NODE_BREAK]    = NULL, // node->node = NULL
    [AST_NODE_CONTINUE] = NULL, // node->node = NULL
    [AST_NODE_RETURN]   = NULL, // node->node = expr
    [AST_NODE_LIST]     = NULL,
};

int compile(struct ast_node* node, struct chunk* chunk,
            struct compile_state* state) {
    return compile_funcs[node->type](node->node, chunk, state);
}
