#include "compile.h"
#include "ast_node.h"
#include "memory.h"
#include "string.h"
#include "value.h"
#include "function.h"
#include <string.h>

// #define HG_ENABLE_WARNING

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
    const char* id;
    struct local* prev;
};

// find local only in the same scope
static int find_scope_local(struct compiler_context* ctx, const char* id) {
    struct local** lpp = &ctx->local_list;

    while (*lpp != NULL) {
        if ((*lpp)->scope == ctx->scope_depth && strcmp((*lpp)->id, id) == 0)
            return (*lpp)->offset;

        lpp = &(*lpp)->prev;
    }
    return -1;
}

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
static int add_local(struct compiler_context* ctx, const char* id) {
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
    hash_map_init(&ctx->funcs);
    ctx->scope_depth = 0;
    ctx->chk         = chk;
    ctx->frame       = NULL;
    ctx->local_list  = NULL;

    enter_frame(ctx); // init global frame
}

void compiler_context_free(struct compiler_context* ctx) {
    leave_frame(ctx); // free global frame

    hash_map_free(&ctx->global_vars);
    hash_map_free(&ctx->funcs);
    ctx->scope_depth = 0;
    ctx->chk         = NULL;
    ctx->frame       = NULL;
    ctx->local_list  = NULL;
}

static _force_inline_ void enter_scope(struct compiler_context* ctx) {
    ctx->scope_depth++;
}

static _force_inline_ void leave_scope(struct compiler_context* ctx) {
    remove_locals(ctx, ctx->scope_depth--);
}
//< compiler_context

//> helper function
static int find_variable_in_scope(struct compiler_context* ctx,
                                  struct hg_value id, bool* is_local) {
    struct hg_string* str = (struct hg_string*)VAL_AS_OBJ(id);
    if (ctx->scope_depth == 0) {
        // find globals
        *is_local           = false;
        struct hg_value val = hash_map_get(&ctx->global_vars, id);

        return VAL_IS_UNDEF(val) ? -1 : (int)VAL_AS_INT(val);
    } else {
        // find locals
        *is_local = true;
        int off   = find_scope_local(ctx, str->str);
        return off;
    }
}

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
static inline int compile_ast_node(struct compiler_context* ctx,
                                   struct ast_node* node);
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

    rc |= compile_ast_node(ctx, node->left);

    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    chunk_write(ctx->chk, OP_TRUE);

    chunk_write(ctx->chk, OP_JUMP);
    j_patch_pos = chunk_write_word(ctx->chk, 0u);

    // patch jump-if-false
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                     jif_patch_pos);

    rc |= compile_ast_node(ctx, node->right);

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

    rc |= compile_ast_node(ctx, node->left);

    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    rc |= compile_ast_node(ctx, node->right);

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
#define compile_left_right()                      \
    do {                                          \
        rc |= compile_ast_node(ctx, node->left);  \
        rc |= compile_ast_node(ctx, node->right); \
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
        rc = compile_ast_node(ctx, node->right);
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
        rc = compile_ast_node(ctx, node->right);
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
            hg_value_write(*val, stderr, true);
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

    int rc = 0;
    for (size_t i = 0; i < stats->len; i++) {
        struct ast_node* node = stats->arr[i];
        rc |= compile_ast_node(ctx, node);

        if (node->type == AST_NODE_CALL || node->type == AST_NODE_VALUE ||
            node->type == AST_NODE_LIST || node->type == AST_NODE_TUPLE ||
            node->type == AST_NODE_OP || node->type == AST_NODE_ARGS ||
            node->type == AST_NODE_VARS) {
            chunk_write(ctx->chk, OP_POP);
#ifdef HG_ENABLE_WARNING
            fprintf(stderr, "compiler warning: the result of the expression "
                            "was not used\n");
#endif
        }
    }

    return rc;
}

static int compile_ast_node_tuple(struct compiler_context* ctx, void* _tuple) {
    struct ast_node_array* tuple = _tuple;
    switch (tuple->len) {
    case 0: // nil
        chunk_write(ctx->chk, OP_NIL);
        break;
    case 1: // unpack if there is only one element in the tuple
        compile_ast_node(ctx, tuple->arr[0]);
        break;
    default:
        // TODO: Compile tuple
        unimplemented_("TODO: tuple with length of %ld", tuple->len);
    }
    return 0;
}

static int compile_ast_node_index_set(struct compiler_context* ctx,
                                      struct ast_node_index* idx) {
    int rc        = 0;
    uint16_t argc = 3;

    rc |= compile_ast_node(ctx, idx->xs);
    rc |= compile_ast_node(ctx, idx->idx);

    struct hg_value id  = VAL_OBJ(hg_symbol_copy("!index_set", 10));
    struct hg_value val = hash_map_get(&ctx->funcs, id);
    if (VAL_IS_UNDEF(val)) {
        fprintf(stderr, "compiler error: call an undefined function");
        return -1;
    }
    hg_value_free(id);

    uint16_t loc = (uint16_t)VAL_AS_INT(val);

    chunk_write(ctx->chk, OP_CALL);
    chunk_write_word(ctx->chk, loc);
    chunk_write_word(ctx->chk, argc);

    // pop the result of calling !index_set
    chunk_write(ctx->chk, OP_POP);

    return rc;
}

static int compile_ast_node_index_get(struct compiler_context* ctx,
                                      struct ast_node_index* idx) {
    int rc        = 0;
    uint16_t argc = 2;

    rc |= compile_ast_node(ctx, idx->xs);
    rc |= compile_ast_node(ctx, idx->idx);

    struct hg_value id  = VAL_OBJ(hg_symbol_copy("!index_get", 10));
    struct hg_value val = hash_map_get(&ctx->funcs, id);
    if (VAL_IS_UNDEF(val)) {
        fprintf(stderr, "compiler error: call an undefined function");
        return -1;
    }
    hg_value_free(id);

    uint16_t loc = (uint16_t)VAL_AS_INT(val);

    chunk_write(ctx->chk, OP_CALL);
    chunk_write_word(ctx->chk, loc);
    chunk_write_word(ctx->chk, argc);

    return rc;
}

static int compile_ast_node_index(struct compiler_context* ctx, void* idx) {
    return compile_ast_node_index_get(ctx, idx);
}

static int compile_ast_node_args(struct compiler_context* ctx, void* _args) {
    struct ast_node_array* args = _args;

    int rc = 0;
    for (size_t i = 0; i < args->len; i++) {
        if (args->arr[i]->type == AST_NODE_INDEX) {
            rc |= compile_ast_node_index_get(ctx, args->arr[i]->node);
        } else {
            rc |= compile_ast_node(ctx, args->arr[i]);
        }
    }
    return rc;
}

static int compile_define_vars(struct compiler_context* ctx, void* _vars) {
    struct ast_node_array* vars = _vars;

    int rc = 0;

    for (size_t i = 0; i < vars->len; i++) {
        struct hg_value* id = vars->arr[i]->node;

        int loc;
        bool islocal;
        if ((loc = find_variable_in_scope(ctx, *id, &islocal)) != -1) {
            fprintf(stderr, "redefine variable: ");
            hg_value_write(*id, stderr, true);
            fprintf(stderr, "\n");
            return -1;
        } else if (ctx->scope_depth == 0) {
            // define global var
            loc = chunk_add_static(ctx->chk, VAL_UNDEF());
            hash_map_put(&ctx->global_vars, *id, VAL_INT(loc));

            chunk_write(ctx->chk, OP_SET_STATIC);
            chunk_write_word(ctx->chk, loc);
        } else {
            // define local var
            add_local(ctx, ((struct hg_string*)VAL_AS_OBJ(*id))->str);
        }
    }
    return rc;
}

static int compile_set_vars(struct compiler_context* ctx, void* _vars) {
    struct ast_node_array* vars = _vars;

    int rc = 0;

    for (int i = vars->len - 1; i >= 0; i--) {
        if (vars->arr[i]->type == AST_NODE_INDEX) {
            rc |= compile_ast_node_index_set(ctx, vars->arr[i]->node);
            continue;
        }

        struct hg_value* id = vars->arr[i]->node;

        int loc;
        bool islocal;
        if ((loc = find_variable(ctx, *id, &islocal)) != -1) {
            chunk_write(ctx->chk, islocal ? OP_SET_LOCAL : OP_SET_STATIC);
            chunk_write_word(ctx->chk, loc);
        } else {
            fprintf(stderr, "compile error: reference before definition: ");
            hg_value_write(*id, stderr, true);
            fprintf(stderr, "\n");
            rc |= -1;
        }
    }
    return rc;
}

static int compile_ast_node_assign(struct compiler_context* ctx,
                                   void* _assign) {
    struct ast_node_assign* node = _assign;
    // check if args.len == vars.len
    struct ast_node_array* args = node->args->node;
    struct ast_node_array* vars = node->vars->node;

    int rc = 0;

    if (args->len != vars->len) {
        fprintf(stderr,
                "compiler error: expect %ld on the left of assignment but got "
                "%ld\n",
                vars->len, args->len);
        return -1;
    }

    rc |= compile_ast_node(ctx, node->args);
    rc |= compile_set_vars(ctx, node->vars->node);
    return rc;
}

static int compile_ast_node_let(struct compiler_context* ctx, void* _assign) {
    struct ast_node_assign* node = _assign;
    // check if args.len == vars.len
    struct ast_node_array* args = node->args->node;
    struct ast_node_array* vars = node->vars->node;

    if (args->len != vars->len) {
        fprintf(stderr,
                "compiler error: expect %ld on the left of assignment but got "
                "%ld\n",
                vars->len, args->len);
        return -1;
    }

    int rc = 0;
    rc |= compile_ast_node(ctx, node->args);

    rc |= compile_define_vars(ctx, node->vars->node);
    return rc;
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
        rc |= compile_ast_node(ctx, node_if->cond);

        // jump-if-false
        chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
        jif_patch_pos = chunk_write_word(ctx->chk, 0u);
    }

    // block
    if (node_if->stats != NULL) {
        enter_scope(ctx);
        rc |= compile_ast_node(ctx, node_if->stats);
        leave_scope(ctx);
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
        rc |= compile_ast_node(ctx, node_if->opt_else);

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

    rc |= compile_ast_node(ctx, node_while->cond);

    // jump-if-false
    chunk_write(ctx->chk, OP_JUMP_IF_FALSE);
    jif_patch_pos = chunk_write_word(ctx->chk, 0u);

    enter_scope(ctx);
    rc |= compile_ast_node(ctx, node_while->stats);
    leave_scope(ctx);

    // jump-back
    chunk_write(ctx->chk, OP_JUMP_BACK);
    chunk_write_word(ctx->chk, (uint16_t)(ctx->chk->len - jb_pos));

    // patch jump-if-false
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - jif_patch_pos),
                     jif_patch_pos);
    return rc;
}

static int compile_ast_node_func(struct compiler_context* ctx, void* _func) {
    int rc = 0;

    struct ast_node_func* func  = _func;
    struct ast_node_array* vars = func->vars == NULL ? NULL : func->vars->node;
    struct hg_value* id         = func->id->node;

    struct hg_function hg_func = {
        .name       = *id,
        .argc       = vars == NULL ? 0 : vars->len,
        .is_builtin = false,
    };

    // TODO: a better way to execute global code
    chunk_write(ctx->chk, OP_JUMP);
    int j_patch_pos = chunk_write_word(ctx->chk, 0u);

    // TODO: find out why the line below do not work
    // hg_func.as.user_def = ctx->chk->code + ctx->chk->len;
    hg_func.as.user_def = ctx->chk->len;

    int loc = chunk_add_func(ctx->chk, hg_func);
    hash_map_put(&ctx->funcs, *id, VAL_INT(loc));

    enter_frame(ctx);
    enter_scope(ctx);

    if (vars != NULL) {
        rc |= compile_define_vars(ctx, vars);
    }

    if (func->stats != NULL) {
        rc |= compile_ast_node(ctx, func->stats);
    }

    leave_scope(ctx);

    // after poping
    chunk_write(ctx->chk, OP_RET);

    leave_frame(ctx);

    // patch jump
    chunk_patch_word(ctx->chk, (uint16_t)(ctx->chk->len - j_patch_pos),
                     j_patch_pos);

    return rc;
}

static int compile_ast_node_call(struct compiler_context* ctx, void* _call) {
    struct ast_node_call* call = _call;

    int rc        = 0;
    uint16_t argc = 0;
    if (call->args != NULL) {
        rc |= compile_ast_node(ctx, call->args);

        struct ast_node_array* args = call->args->node;

        if (args->len > UINT16_MAX + 1u) {
            fprintf(stderr,
                    "compiler error: cannot have more than %u arguments",
                    UINT16_MAX + 1u);
            return -1;
        }
        argc = (uint16_t)args->len;
    }

    // TODO: function may be something like an item in a list, or a return value
    // rc |= compile_ast_node(ctx, call->func);
    struct hg_value* id = call->func->node;
    struct hg_value val = hash_map_get(&ctx->funcs, *id);
    if (VAL_IS_UNDEF(val)) {
        fprintf(stderr, "compiler error: call an undefined function");
        return -1;
    }

    uint16_t loc = (uint16_t)VAL_AS_INT(val);

    chunk_write(ctx->chk, OP_CALL);
    chunk_write_word(ctx->chk, loc);
    chunk_write_word(ctx->chk, argc);

    return rc;
}

static int compile_ast_node_return(struct compiler_context* ctx, void* _ret) {
    int rc = 0;

    struct ast_node* ret = _ret;

    if (ret == NULL) {
        chunk_write(ctx->chk, OP_RET);
    } else {
        rc |= compile_ast_node(ctx, ret);
        chunk_write(ctx->chk, OP_RETV);
    }

    return rc;
}

static int compile_ast_node_list(struct compiler_context* ctx, void* _list) {
    struct ast_node_array* list = _list;

    int rc        = 0;
    uint16_t argc = 0;
    if (list != NULL) {
        rc |= compile_ast_node_args(ctx, list);

        if (list->len > UINT16_MAX + 1u) {
            fprintf(stderr,
                    "compiler error: cannot have more than %u elements while "
                    "compiling a list",
                    UINT16_MAX + 1u);
            return -1;
        }
        argc = (uint16_t)list->len;
    }

    struct hg_value id  = VAL_OBJ(hg_symbol_copy("List", 4));
    struct hg_value val = hash_map_get(&ctx->funcs, id);
    if (VAL_IS_UNDEF(val)) {
        fprintf(stderr, "compiler error: call an undefined function");
        return -1;
    }
    hg_value_free(id);

    uint16_t loc = (uint16_t)VAL_AS_INT(val);

    chunk_write(ctx->chk, OP_CALL);
    chunk_write_word(ctx->chk, loc);
    chunk_write_word(ctx->chk, argc);

    return rc;
}

static int compile_ast_node_map(struct compiler_context* ctx, void* _map) {
    struct ast_node_array* map = _map;
    assert(map != NULL);

    int rc        = 0;
    uint16_t argc = 0;

    rc |= compile_ast_node_args(ctx, map);

    if (map->len > UINT16_MAX + 1u) {
        fprintf(stderr,
                "compiler error: cannot have more than %u elements while "
                "compiling a list",
                UINT16_MAX + 1u);
        return -1;
    }
    argc = (uint16_t)map->len;

    struct hg_value id  = VAL_OBJ(hg_symbol_copy("Map", 3));
    struct hg_value val = hash_map_get(&ctx->funcs, id);
    if (VAL_IS_UNDEF(val)) {
        fprintf(stderr, "compiler error: call an undefined function");
        return -1;
    }
    hg_value_free(id);

    uint16_t loc = (uint16_t)VAL_AS_INT(val);

    chunk_write(ctx->chk, OP_CALL);
    chunk_write_word(ctx->chk, loc);
    chunk_write_word(ctx->chk, argc);

    return rc;
}

// compile_funcs is a static array of const pointer to function
static int (*const compile_funcs[])(struct compiler_context*, void*) = {
    [AST_NODE_OP]       = compile_ast_node_op,
    [AST_NODE_VALUE]    = compile_ast_node_value, // node->node = hg_value
    [AST_NODE_STATS]    = compile_ast_node_stats,
    [AST_NODE_TUPLE]    = compile_ast_node_tuple,
    [AST_NODE_ASSIGN]   = compile_ast_node_assign,
    [AST_NODE_LET]      = compile_ast_node_let,
    [AST_NODE_VARS]     = NULL,
    [AST_NODE_ARGS]     = compile_ast_node_args,
    [AST_NODE_IF]       = compile_ast_node_if,
    [AST_NODE_WHILE]    = compile_ast_node_while,
    [AST_NODE_FOR]      = NULL,
    [AST_NODE_CALL]     = compile_ast_node_call,
    [AST_NODE_FUNC]     = compile_ast_node_func,
    [AST_NODE_BREAK]    = NULL,                    // node->node = NULL
    [AST_NODE_CONTINUE] = NULL,                    // node->node = NULL
    [AST_NODE_RETURN]   = compile_ast_node_return, // node->node = expr
    [AST_NODE_LIST]     = compile_ast_node_list,
    [AST_NODE_MAP]      = compile_ast_node_map,
    [AST_NODE_INDEX]    = compile_ast_node_index,
};

static inline int compile_ast_node(struct compiler_context* ctx,
                                   struct ast_node* node) {
    return compile_funcs[node->type](ctx, node->node);
}
//< compile ast_node

static void import_builtin(struct compiler_context* ctx) {
    int count;
    const struct hg_function* funcs = hg_function_get_builtins(&count);

    for (int i = 0; i < count; i++) {
        int loc = chunk_add_func(ctx->chk, funcs[i]);
        hash_map_put(&ctx->funcs, funcs[i].name, VAL_INT(loc));
    }
}

int compile(struct compiler_context* ctx, struct ast_node* node) {
    int rc = 0;

    import_builtin(ctx);

    rc = compile_ast_node(ctx, node);

    chunk_write(ctx->chk, OP_QUIT);

    return rc;
}
