// TODO: Check the number of arguments while compiling fucntion call
#include "compile.h"
#include "ast_node.h"
#include "common.h"

/* a OR b
     a
 +---jump-if-false
 |   op_true
 |   jump --+
 +-->b      |
        <---+
 */
static int compile_ast_node_or(struct compiler_context* ctx,
                               struct ast_node_op* node, struct chunk* chk) {
    int rc            = 0;
    int jif_patch_pos = -1;
    int j_patch_pos   = -1;

    rc |= compile(ctx, node->left, chk);

    chunk_write(chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(chk, 0u);

    chunk_write(chk, OP_TRUE);

    chunk_write(chk, OP_JUMP);
    j_patch_pos = chunk_write_word(chk, 0u);

    // patch jump-if-false
    chunk_patch_word(chk, (uint16_t)(chk->len - jif_patch_pos), jif_patch_pos);

    rc |= compile(ctx, node->right, chk);

    // patch jump
    chunk_patch_word(chk, (uint16_t)(chk->len - j_patch_pos), j_patch_pos);

    return rc;
}

/* a AND b
      a
  +---jump-if-false
  |   b
  |   jump ----+
  +-->op_false |
           <---+
 */
static int compile_ast_node_and(struct compiler_context* ctx,
                                struct ast_node_op* node, struct chunk* chk) {
    int rc            = 0;
    int j_patch_pos   = -1;
    int jif_patch_pos = -1;

    rc |= compile(ctx, node->left, chk);

    chunk_write(chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(chk, 0u);

    rc |= compile(ctx, node->right, chk);

    chunk_write(chk, OP_JUMP);
    j_patch_pos = chunk_write_word(chk, 0u);

    // patch jump-if-false
    chunk_patch_word(chk, (uint16_t)(chk->len - jif_patch_pos), jif_patch_pos);

    chunk_write(chk, OP_FALSE);

    // patch jump
    chunk_patch_word(chk, (uint16_t)(chk->len - j_patch_pos), j_patch_pos);

    return rc;
}

static int compile_ast_node_op(struct compiler_context* ctx, void* _node,
                               struct chunk* chk) {

    int rc = 0;
#define compile_left_right()                  \
    do {                                      \
        rc |= compile(ctx, node->left, chk);  \
        rc |= compile(ctx, node->right, chk); \
    } while (0)

    struct ast_node_op* node = _node;

    switch (node->op) {
    case AST_NODE_OP_AND:
        rc = compile_ast_node_and(ctx, node, chk);
        break;
    case AST_NODE_OP_OR:
        rc = compile_ast_node_or(ctx, node, chk);
        break;
    case AST_NODE_OP_NOT:
        rc = compile(ctx, node->right, chk);
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
        rc = compile(ctx, node->right, chk);
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
    return rc;
#undef compile_left_right
}

static int compile_ast_node_variable(struct compiler_context* ctx,
                                     struct hg_value id, struct chunk* chk) {
    if (ctx->scope_depth == 0) {
        // global
        struct hg_value val = hash_map_get(&ctx->global_vars, id);
        uint16_t loc;
        if (VAL_IS_UNDEF(val)) {
            fprintf(stderr, "compile error: reference before definition `");
            hg_value_write(id, stderr);
            fprintf(stderr, "`\n");
            return -1;
        }
        loc = (uint16_t)VAL_AS_INT(val);

        chunk_write(chk, OP_GET_STATIC);
        chunk_write_word(chk, loc);
    } else {
        // TODO: Compile local variable (get)
        unimplemented_("local");
    }
    return 0;
}

static int compile_ast_node_value(struct compiler_context* ctx, void* _value,
                                  struct chunk* chk) {
    struct hg_value* val = _value;

    if (VAL_IS_BOOL(*val)) {
        chunk_write(chk, VAL_AS_BOOL(*val) ? OP_TRUE : OP_FALSE);
    } else if (VAL_IS_OBJ(*val) && (VAL_AS_OBJ(*val)->type == HG_OBJ_SYMBOL)) {
        return compile_ast_node_variable(ctx, *val, chk);
    } else {
        uint16_t loc = chunk_add_const(chk, *val);
        chunk_write(chk, OP_GET_CONST);
        chunk_write_word(chk, loc);
    }
    return 0;
}

static int compile_ast_node_stats(struct compiler_context* ctx, void* _stats,
                                  struct chunk* chk) {
    struct ast_node_array* stats = _stats;

    int rc = 0;
    for (size_t i = 0; i < stats->len; i++) {
        rc |= compile(ctx, stats->arr[i], chk);
    }
    return rc;
}

static int compile_ast_node_tuple(struct compiler_context* ctx, void* _tuple,
                                  struct chunk* chk) {
    struct ast_node_array* tuple = _tuple;
    switch (tuple->len) {
    case 0: // nil
        chunk_write(chk, OP_NIL);
        break;
    case 1: // unpack if there is only one element in the tuple
        compile(ctx, tuple->arr[0], chk);
        break;
    default:
        // TODO: Compile tuple
        unimplemented_("TODO: tuple with length of %ld", tuple->len);
    }
    return 0;
}

static int compile_ast_node_args(struct compiler_context* ctx, void* _args,
                                 struct chunk* chk) {
    struct ast_node_array* args = _args;

    int rc = 0;
    for (int i = args->len - 1; i >= 0; i--) {
        // push inversely
        rc |= compile(ctx, args->arr[i], chk);
    }
    return rc;
}

static int compile_ast_node_vars(struct compiler_context* ctx, void* _vars,
                                 struct chunk* chk) {
    struct ast_node_array* vars = _vars;

    int rc = 0;

    for (size_t i = 0; i < vars->len; i++) {
        struct hg_value* id = vars->arr[i]->node;
        if (ctx->scope_depth == 0) {
            // global
            struct hg_value val = hash_map_get(&ctx->global_vars, *id);
            uint16_t loc;
            if (VAL_IS_UNDEF(val)) {
                loc = chunk_add_static(chk, val);
                hash_map_put(&ctx->global_vars, *id, VAL_INT(loc));
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

static int compile_ast_node_assign(struct compiler_context* ctx, void* _assign,
                                   struct chunk* chk) {
    struct ast_node_assign* node = _assign;
    compile(ctx, node->args, chk);
    compile(ctx, node->vars, chk);
    return 0;
}

/* IF-ELSE-IF-ELSE:
 *     if-cont
 * +---jump-if-false
 * |   if-block
 * |   jump  ----------+
 * +-->else-if-cont    |
 * +---jump-if-false   |
 * |   else-if-block   |
 * |   jump  ----------+
 * +-->else-block      |
 *           <---------+
 */
static int compile_ast_node_if(struct compiler_context* ctx, void* _if,
                               struct chunk* chk) {
    struct ast_node_if* node_if = _if;
    int rc                      = 0;
    int jif_patch_pos           = -1; // jump if false
    int j_patch_pos             = -1; // jump

    if (node_if->cond != NULL) {
        rc |= compile(ctx, node_if->cond, chk);

        // jump-if-false
        chunk_write(chk, OP_JUMP_IF_FALSE);
        jif_patch_pos = chunk_write_word(chk, 0u);
    }

    // block
    if (node_if->stats != NULL) {
        rc |= compile(ctx, node_if->stats, chk);
    }

    // jump: needed only of there is *else-if* or *else*
    if (node_if->opt_else != NULL) {
        chunk_write(chk, OP_JUMP);
        j_patch_pos = chunk_write_word(chk, 0u);
    }

    // patch jump-if-false
    if (node_if->cond != NULL) {
        chunk_patch_word(chk, (uint16_t)(chk->len - jif_patch_pos),
                         jif_patch_pos);
    }

    // *else-if* and *else*
    if (node_if->opt_else != NULL) {
        rc |= compile(ctx, node_if->opt_else, chk);

        // patch jump
        chunk_patch_word(chk, (uint16_t)(chk->len - j_patch_pos), j_patch_pos);
    }

    return rc;
}

// compile_funcs is a static array of const pointer to function
static int (*const compile_funcs[])(struct compiler_context*, void*,
                                    struct chunk*) = {
    [AST_NODE_OP]       = compile_ast_node_op,
    [AST_NODE_VALUE]    = compile_ast_node_value, // node->node = hg_value
    [AST_NODE_STATS]    = compile_ast_node_stats,
    [AST_NODE_TUPLE]    = compile_ast_node_tuple,
    [AST_NODE_ASSIGN]   = compile_ast_node_assign,
    [AST_NODE_VARS]     = compile_ast_node_vars,
    [AST_NODE_ARGS]     = compile_ast_node_args,
    [AST_NODE_IF]       = compile_ast_node_if,
    [AST_NODE_FOR]      = NULL,
    [AST_NODE_CALL]     = NULL,
    [AST_NODE_FUNC]     = NULL,
    [AST_NODE_WHILE]    = NULL,
    [AST_NODE_BREAK]    = NULL, // node->node = NULL
    [AST_NODE_CONTINUE] = NULL, // node->node = NULL
    [AST_NODE_RETURN]   = NULL, // node->node = expr
    [AST_NODE_LIST]     = NULL,
};

int compile(struct compiler_context* ctx, struct ast_node* node,
            struct chunk* chunk) {
    return compile_funcs[node->type](ctx, node->node, chunk);
}
