#include "compile.h"
#include "memory.h"
#include "string.h"
#include "value.h"
#include <string.h>

//> frame_context
struct frame_context {
    int local_cnt;
    struct frame_context* prev;
};
static void enter_frame(struct compiler_context* ctx) {
    struct frame_context* p = hg_alloc_(struct frame_context);

    *p = (struct frame_context){0, ctx->frame};

    ctx->frame = p;
}

static void leave_frame(struct compiler_context* ctx) {
    struct frame_context* t = ctx->frame;

    ctx->frame = t->prev;

    hg_free_(struct frame_context, t);
}
//< frame_context

//> local
struct local {
    int offset; // offset int the function stack frame
    int scope;
    const_shared_char_p id;
    struct local* prev;
};

// return the offset for success, -1 for error
static int find_local(struct compiler_context* ctx, const char* id) {
    struct local** lpp = &ctx->local_list;

    while (*lpp != NULL) {
        if (strcmp((*lpp)->id, id) == 0)
            return (*lpp)->offset;

        lpp = &(*lpp)->prev;
    }
    return -1;
}

// return the offset
static int add_local(struct compiler_context* ctx, const_shared_char_p id) {
    struct local* p = hg_alloc_(struct local);

    *p = (struct local){
        .id     = id,
        .scope  = ctx->scope_depth,
        .offset = ctx->frame->local_cnt++,
        .prev   = ctx->local_list,
    };

    ctx->local_list = p;

    return p->offset;
}

// remove local vars with the given `scope`
static void remove_locals(struct compiler_context* ctx, int scope) {
    struct local** lpp = &ctx->local_list;

    while (*lpp != NULL && (*lpp)->scope == scope) {
        // pop frame
        chunk_write(ctx->chk, OP_POP);
        ctx->frame->local_cnt--;

        struct local* p = *lpp;

        *lpp = (*lpp)->prev; // remove

        hg_free_(struct local, p);
    }
}
//< local

//> compiler_context
void compiler_context_init(struct compiler_context* ctx, shared_chunk_p chk) {
    hash_map_init(&ctx->global_vars);
    ctx->scope_depth = -1;
    ctx->chk         = chk;
    ctx->frame       = NULL;
    ctx->local_list  = NULL;

    enter_frame(ctx); // init global frame
}

void compiler_context_free(struct compiler_context* ctx) {
    leave_frame(ctx); // free global frame

    hash_map_free(&ctx->global_vars);
    ctx->scope_depth = 0;
    ctx->chk         = NULL;
    ctx->frame       = NULL;
    ctx->local_list  = NULL;
}

static _force_inline_ void entry_scope(struct compiler_context* ctx) {
    ctx->scope_depth++;
}

static _force_inline_ void leave_scope(struct compiler_context* ctx) {
    remove_locals(ctx, ctx->scope_depth--);
}
//< compiler_context

//> helper function
static int find_variable(struct compiler_context* ctx, struct hg_value id,
                         bool* is_local) {
    struct hg_string* str = (struct hg_string*)VAL_AS_OBJ(id);

    // find locals
    *is_local = true;
    int off   = find_local(ctx, str->str);
    if (off != -1)
        return off;

    // find globals
    *is_local           = false;
    struct hg_value val = hash_map_get(&ctx->global_vars, id);
    if (VAL_IS_UNDEF(val)) {
        return -1;
    }

    return (int)VAL_AS_INT(val);
}
//< helper function

//> compile ast_node
/* a OR b
     a
 +---jump-if-false
 |   op_true
 |   jump --+
 +-->b      |
        <---+
 */
static int compile_ast_node_or(struct compiler_context* ctx,
                               struct ast_node_op* node) {
    int rc            = 0;
    int jif_patch_pos = -1;
    int j_patch_pos   = -1;

    rc |= compile(ctx, node->left);

    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    chunk_write(ctx->chk, OP_TRUE);

    chunk_write(ctx->chk, OP_JUMP);
    j_patch_pos = chunk_write_word(ctx->chk, 0u);

    // patch jump-if-false
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                     jif_patch_pos);

    rc |= compile(ctx, node->right);

    // patch jump
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - j_patch_pos),
                     j_patch_pos);

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
                                struct ast_node_op* node) {
    int rc            = 0;
    int j_patch_pos   = -1;
    int jif_patch_pos = -1;

    rc |= compile(ctx, node->left);

    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    rc |= compile(ctx, node->right);

    chunk_write(ctx->chk, OP_JUMP);
    j_patch_pos = chunk_write_word(ctx->chk, 0u);

    // patch jump-if-false
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                     jif_patch_pos);

    chunk_write(ctx->chk, OP_FALSE);

    // patch jump
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - j_patch_pos),
                     j_patch_pos);

    return rc;
}

static int compile_ast_node_op(struct compiler_context* ctx, void* _node) {

    int rc = 0;
#define compile_left_right()             \
    do {                                 \
        rc |= compile(ctx, node->left);  \
        rc |= compile(ctx, node->right); \
    } while (0)

    struct ast_node_op* node = _node;

    switch (node->op) {
    case AST_NODE_OP_AND:
        rc = compile_ast_node_and(ctx, node);
        break;
    case AST_NODE_OP_OR:
        rc = compile_ast_node_or(ctx, node);
        break;
    case AST_NODE_OP_NOT:
        rc = compile(ctx, node->right);
        chunk_write(ctx->chk, OP_NOT);
        break;
    case AST_NODE_OP_NEQ:
        compile_left_right();
        chunk_write(ctx->chk, OP_EQUAL);
        chunk_write(ctx->chk, OP_NOT);
        break;
    case AST_NODE_OP_EQ:
        compile_left_right();
        chunk_write(ctx->chk, OP_EQUAL);
        break;
    case AST_NODE_OP_GE:
        compile_left_right();
        chunk_write(ctx->chk, OP_GREATER_EQUAL);
        break;
    case AST_NODE_OP_LE:
        compile_left_right();
        chunk_write(ctx->chk, OP_LESS_EQUAL);
        break;
    case AST_NODE_OP_GT:
        compile_left_right();
        chunk_write(ctx->chk, OP_GREATER);
        break;
    case AST_NODE_OP_LS:
        compile_left_right();
        chunk_write(ctx->chk, OP_LESS);
        break;
    case AST_NODE_OP_NEG:
        rc = compile(ctx, node->right);
        chunk_write(ctx->chk, OP_NEGATE);
        break;
    case AST_NODE_OP_ADD:
        compile_left_right();
        chunk_write(ctx->chk, OP_ADD);
        break;
    case AST_NODE_OP_SUB:
        compile_left_right();
        chunk_write(ctx->chk, OP_SUBTRACT);
        break;
    case AST_NODE_OP_MUL:
        compile_left_right();
        chunk_write(ctx->chk, OP_MULTIPLY);
        break;
    case AST_NODE_OP_DIV:
        compile_left_right();
        chunk_write(ctx->chk, OP_DIVIDE);
        break;
    case AST_NODE_OP_MOD:
        compile_left_right();
        chunk_write(ctx->chk, OP_MODULO);
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

static int compile_ast_node_value(struct compiler_context* ctx, void* _value) {
    struct hg_value* val = _value;

    int rc = 0;

    if (VAL_IS_BOOL(*val)) {
        chunk_write(ctx->chk, VAL_AS_BOOL(*val) ? OP_TRUE : OP_FALSE);
    } else if (VAL_IS_OBJ(*val) && (VAL_AS_OBJ(*val)->type == HG_OBJ_SYMBOL)) {
        // TODO: splite id and value
        bool islocal;
        int loc;
        if ((loc = find_variable(ctx, *val, &islocal)) == -1) {
            fprintf(stderr, "compile error: reference before definition `");
            hg_value_write(*val, stderr);
            fprintf(stderr, "`\n");
            rc = -1;
        } else {
            chunk_write(ctx->chk, islocal ? OP_GET_LOCAL : OP_GET_STATIC);
            chunk_write_word(ctx->chk, loc);
        }
    } else {
        uint16_t loc = chunk_add_const(ctx->chk, *val);
        chunk_write(ctx->chk, OP_GET_CONST);
        chunk_write_word(ctx->chk, loc);
    }
    return rc;
}

static int compile_ast_node_stats(struct compiler_context* ctx, void* _stats) {
    struct ast_node_array* stats = _stats;

    entry_scope(ctx);

    int rc = 0;
    for (size_t i = 0; i < stats->len; i++) {
        rc |= compile(ctx, stats->arr[i]);
    }

    leave_scope(ctx);
    return rc;
}

static int compile_ast_node_tuple(struct compiler_context* ctx, void* _tuple) {
    struct ast_node_array* tuple = _tuple;
    switch (tuple->len) {
    case 0: // nil
        chunk_write(ctx->chk, OP_NIL);
        break;
    case 1: // unpack if there is only one element in the tuple
        compile(ctx, tuple->arr[0]);
        break;
    default:
        // TODO: Compile tuple
        unimplemented_("TODO: tuple with length of %ld", tuple->len);
    }
    return 0;
}

static int compile_ast_node_args(struct compiler_context* ctx, void* _args) {
    struct ast_node_array* args = _args;

    int rc = 0;
    for (int i = args->len - 1; i >= 0; i--) {
        // push inversely
        rc |= compile(ctx, args->arr[i]);
    }
    return rc;
}

static int compile_ast_node_vars(struct compiler_context* ctx, void* _vars) {
    struct ast_node_array* vars = _vars;

    int rc = 0;

    for (size_t i = 0; i < vars->len; i++) {
        struct hg_value* id = vars->arr[i]->node;

        int loc;
        bool islocal;
        if ((loc = find_variable(ctx, *id, &islocal)) == -1) {
            // not found

            if (ctx->scope_depth == 0) {
                // global
                islocal = false;
                loc     = chunk_add_static(ctx->chk, VAL_UNDEF());
                hash_map_put(&ctx->global_vars, *id, VAL_INT(loc));
            } else {
                // local
                islocal = true;
                loc = add_local(ctx, ((struct hg_string*)VAL_AS_OBJ(*id))->str);
            }
        }

        chunk_write(ctx->chk, islocal ? OP_SET_LOCAL : OP_SET_STATIC);
        chunk_write_word(ctx->chk, loc);
    }
    return rc;
}

static int compile_ast_node_assign(struct compiler_context* ctx,
                                   void* _assign) {
    struct ast_node_assign* node = _assign;
    compile(ctx, node->args);
    compile(ctx, node->vars);
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
static int compile_ast_node_if(struct compiler_context* ctx, void* _if) {
    struct ast_node_if* node_if = _if;
    int rc                      = 0;
    int jif_patch_pos           = -1; // jump if false
    int j_patch_pos             = -1; // jump

    if (node_if->cond != NULL) {
        rc |= compile(ctx, node_if->cond);

        // jump-if-false
        chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
        jif_patch_pos = chunk_write_word(ctx->chk, 0u);
    }

    // block
    if (node_if->stats != NULL) {
        rc |= compile(ctx, node_if->stats);
    }

    // jump: needed only of there is *else-if* or *else*
    if (node_if->opt_else != NULL) {
        chunk_write(ctx->chk, OP_JUMP);
        j_patch_pos = chunk_write_word(ctx->chk, 0u);
    }

    // patch jump-if-false
    if (node_if->cond != NULL) {
        chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                         jif_patch_pos);
    }

    // *else-if* and *else*
    if (node_if->opt_else != NULL) {
        rc |= compile(ctx, node_if->opt_else);

        // patch jump
        chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - j_patch_pos),
                         j_patch_pos);
    }

    return rc;
}

/*
    WHILE:
      cont         <--+
  +---jump-if-false   |
  |   while-block     |
  |   jump-back    ---+
  +-->
 */
static int compile_ast_node_while(struct compiler_context* ctx, void* _while) {
    int rc            = 0;
    int jb_pos        = -1;
    int jif_patch_pos = -1;

    struct ast_node_while* node_while = _while;

    jb_pos = ctx->chk->len; // jump-back position

    rc |= compile(ctx, node_while->cond);

    // jump-if-false
    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    rc |= compile(ctx, node_while->stats);

    // jump-back
    chunk_write(ctx->chk, OP_JUMP_BACK);
    chunk_write_word(ctx->chk, (uint16_t)(ctx->chk->len - jb_pos));

    // patch jump-if-false
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                     jif_patch_pos);
    return rc;
}

// compile_funcs is a static array of const pointer to function
static int (*const compile_funcs[])(struct compiler_context*, void*) = {
    [AST_NODE_OP]       = compile_ast_node_op,
    [AST_NODE_VALUE]    = compile_ast_node_value, // node->node = hg_value
    [AST_NODE_STATS]    = compile_ast_node_stats,
    [AST_NODE_TUPLE]    = compile_ast_node_tuple,
    [AST_NODE_ASSIGN]   = compile_ast_node_assign,
    [AST_NODE_VARS]     = compile_ast_node_vars,
    [AST_NODE_ARGS]     = compile_ast_node_args,
    [AST_NODE_IF]       = compile_ast_node_if,
    [AST_NODE_WHILE]    = compile_ast_node_while,
    [AST_NODE_FOR]      = NULL,
    [AST_NODE_CALL]     = NULL,
    [AST_NODE_FUNC]     = NULL,
    [AST_NODE_BREAK]    = NULL, // node->node = NULL
    [AST_NODE_CONTINUE] = NULL, // node->node = NULL
    [AST_NODE_RETURN]   = NULL, // node->node = expr
    [AST_NODE_LIST]     = NULL,
};
//< compile ast_node

int compile(struct compiler_context* ctx, struct ast_node* node) {
    return compile_funcs[node->type](ctx, node->node);
}
