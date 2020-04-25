#include "ast_node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hedgehog.h"

#define hg_malloc(size)                                      \
    ({                                                       \
        size_t __size = size;                                \
        void* p       = malloc(__size);                      \
        if (p == NULL) {                                     \
            fprintf(stderr, "malloc error: %s\n", __func__); \
        }                                                    \
        p;                                                   \
    })

#define hg_realloc(p, size)                                  \
    ({                                                       \
        size_t __size = size;                                \
        void* __p     = p;                                   \
        void* new_p   = realloc(__p, __size);                \
        if (new_p == NULL) {                                 \
            fprintf(stderr, "realloc error:%s\n", __func__); \
        }                                                    \
        new_p;                                               \
    })

#define assert_expr(node)                                       \
    {                                                           \
        enum ast_node_type type = node->type;                   \
        assert(type == AST_NODE_VALUE || type == AST_NODE_OP || \
               type == AST_NODE_CALL);                          \
    }

/** ast_node_new: malloc a new ast_node and set its type
 */
static inline struct ast_node* ast_node_new(enum ast_node_type type) {
    struct ast_node* node = hg_malloc(sizeof(struct ast_node));

    node->node = NULL;
    node->type = type;
    return node;
}

struct ast_node* ast_node_op_new(enum ast_node_op_type type,
                                 struct ast_node* left,
                                 struct ast_node* right) {
    if (left == NULL)
        assert(type == AST_NODE_OP_SUB);
    else
        assert_expr(left);
    assert_expr(right);

    struct ast_node* node       = ast_node_new(AST_NODE_OP);
    struct ast_node_op* node_op = hg_malloc(sizeof(struct ast_node_op));

    node_op->op    = type;
    node_op->left  = left;
    node_op->right = right;

    node->node = node_op;
    return node;
}

struct ast_node* ast_node_if_new(struct ast_node* cond, struct ast_node* stats,
                                 struct ast_node* opt_else) {
    if (opt_else != NULL)
        assert(opt_else->type == AST_NODE_IF);
    if (cond != NULL)
        assert_expr(cond);

    struct ast_node* node       = ast_node_new(AST_NODE_IF);
    struct ast_node_if* node_if = hg_malloc(sizeof(struct ast_node_if));

    node_if->cond     = cond;
    node_if->stats    = stats;
    node_if->opt_else = opt_else;

    node->node = node_if;
    return node;
}

struct ast_node* ast_node_call_new(struct ast_node* id,
                                   struct ast_node* tuple) {
    assert(id->type == AST_NODE_VALUE || id->type == AST_NODE_CALL);
    assert(tuple->type == AST_NODE_VALUE);

    struct ast_node* node           = ast_node_new(AST_NODE_CALL);
    struct ast_node_call* node_call = hg_malloc(sizeof(struct ast_node_call));

    node_call->id    = id;
    node_call->tuple = tuple;

    node->node = node_call;
    return node;
}

struct ast_node* ast_node_func_new(struct ast_node* id, struct ast_node* vars,
                                   struct ast_node* stats) {
    assert(id->type == AST_NODE_VALUE);
    if (vars != NULL)
        assert(vars->type == AST_NODE_VARS);
    if (stats != NULL)
        assert(stats->type == AST_NODE_STATS);

    struct ast_node* node           = ast_node_new(AST_NODE_FUNC);
    struct ast_node_func* node_func = hg_malloc(sizeof(struct ast_node_func));

    node_func->id    = id;
    node_func->stats = stats;
    node_func->vars  = vars;

    node->node = node_func;
    return node;
}

struct ast_node* ast_node_array_new(enum ast_node_type type,
                                    struct ast_node* first) {
    struct ast_node* node           = ast_node_new(type);
    struct ast_node_array* node_arr = hg_malloc(sizeof(struct ast_node_array));

    node_arr->capacity = 1;
    node_arr->len      = 1;
    node_arr->arr      = hg_malloc(sizeof(struct ast_node*));

    node_arr->arr[0] = first;

    node->node = node_arr;
    return node;
}
void ast_node_array_add(struct ast_node* arr, struct ast_node* item) {
    struct ast_node_array* node_arr = arr->node;

    assert(node_arr->capacity >= node_arr->len);
    if (node_arr->capacity == node_arr->len) {
        node_arr->capacity *= 2;
        node_arr->arr = hg_realloc(
            node_arr->arr, sizeof(struct ast_node*) * node_arr->capacity);
    }

    node_arr->arr[node_arr->len++] = item;
}

struct ast_node* ast_node_assign_new(struct ast_node* vars,
                                     struct ast_node* args) {
    assert(vars->type == AST_NODE_VARS);
    assert(args->type == AST_NODE_ARGS);

    struct ast_node* node = ast_node_new(AST_NODE_ASSIGN);
    struct ast_node_assign* node_assign =
        hg_malloc(sizeof(struct ast_node_assign));

    node_assign->vars = vars;
    node_assign->args = args;

    node->node = node_assign;
    return node;
}

struct ast_node* ast_node_return_new(struct ast_node* expr) {
    if (expr != NULL)
        assert_expr(expr);

    struct ast_node* node = ast_node_new(AST_NODE_RETURN);
    struct ast_node_return* node_ret =
        hg_malloc(sizeof(struct ast_node_return));

    node_ret->expr = expr;

    node->node = node_ret;
    return node;
}

struct ast_node* ast_node_while_new(struct ast_node* cond,
                                    struct ast_node* stats) {
    assert_expr(cond);
    if (stats != NULL)
        assert(stats->type == AST_NODE_STATS);

    struct ast_node* node = ast_node_new(AST_NODE_WHILE);
    struct ast_node_while* node_while =
        hg_malloc(sizeof(struct ast_node_while));

    node_while->cond  = cond;
    node_while->stats = stats;

    node->node = node_while;
    return node;
}

struct ast_node* ast_node_for_new(struct ast_node* vars,
                                  struct ast_node* iterator,
                                  struct ast_node* stats) {
    assert(vars->type == AST_NODE_VARS);
    assert_expr(iterator);
    assert(stats->type == AST_NODE_STATS);

    struct ast_node* node         = ast_node_new(AST_NODE_FOR);
    struct ast_node_for* node_for = hg_malloc(sizeof(struct ast_node_for));

    node_for->vars     = vars;
    node_for->iterator = iterator;
    node_for->stats    = stats;

    node->node = node_for;
    return node;
}

struct ast_node* ast_node_loopctrl_new(enum ast_node_type type) {
    assert(type == AST_NODE_CONTINUE || type == AST_NODE_BREAK);

    return ast_node_new(type);
}

struct ast_node* ast_node_int_new(int v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_INT;
    value->v.i  = v;

    node->node = value;
    return node;
}

struct ast_node* ast_node_float_new(double v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_FLOAT;
    value->v.f  = v;

    node->node = value;
    return node;
}

struct ast_node* ast_node_bool_new(int v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_BOOL;
    value->v.b  = v;

    node->node = value;
    return node;
}

struct ast_node* ast_node_nil_new() {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_NIL;

    node->node = value;
    return node;
}

struct ast_node* ast_node_str_new(const char* s) {
    return ast_node_str_len_new(s, strlen(s));
}

struct ast_node* ast_node_str_len_new(const char* s, size_t len) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    char* t = hg_malloc(len + 1);
    strncpy(t, s, len);

    value->type = HG_VALUE_STRING;
    value->v.p  = t;

    node->node = value;
    return node;
}

struct ast_node* ast_node_id_new(const char* s) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    size_t len = strlen(s);
    char* t    = hg_malloc(len + 1);

    strncpy(t, s, len);

    value->type = HG_VALUE_ID;
    value->v.p  = t;

    node->node = value;
    return node;
}

struct ast_node* ast_node_list_new(struct ast_node* args) {
    if (args != NULL)
        assert(args->type == AST_NODE_ARGS);

    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_LIST;
    value->v.p  = args;

    node->node = value;
    return node;
}

struct ast_node* ast_node_tuple_new(struct ast_node* args) {
    if (args != NULL)
        assert(args->type == AST_NODE_ARGS);

    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_malloc(sizeof(struct hg_value));

    value->type = HG_VALUE_TUPLE;
    value->v.p  = args;

    node->node = value;
    return node;
}
