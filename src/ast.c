#include "ast.h"
#include "memory.h"

#define _new(x, t) t* x = hg_alloc(sizeof(t))

#define _raw(head_ptr, t)                                           \
    ({                                                              \
        ensure_type_(head_ptr, struct hg_ast_node*);                \
        ((t*)((intptr_t)(head_ptr)-struct_field_offset_(t, node))); \
    })

#define _raw_to(head_ptr, t, var) t* var = _raw((head_ptr), t)

#ifdef HG_DEBUG
static const size_t _start_array_cap = 1;
#else
static const size_t _start_array_cap = 4;
#endif

struct hg_ast_node* hg_ast_node_array_new(struct hg_parser* p) {
    struct hg_ast_node_array* arr =
        hg_alloc(sizeof(struct hg_ast_node_array) +
                 sizeof(struct hg_ast_node*) * _start_array_cap);

    arr->node.type = AST_NODE_ARRAY;
    arr->node.line = p->lineno;
    arr->cap       = _start_array_cap;
    arr->len       = 0;

    return &arr->node;
}

struct hg_ast_node* hg_ast_node_array_append(struct hg_parser* p,
                                             struct hg_ast_node* _arr,
                                             struct hg_ast_node* node) {
    assert(_arr->type == AST_NODE_ARRAY);
    _raw_to(_arr, struct hg_ast_node_array, arr);

    if (arr->len >= arr->cap) {
        arr->cap *= 2;
        arr = hg_realloc(arr, sizeof(struct hg_ast_node_array) +
                                  sizeof(struct hg_ast_node*) * arr->cap);
    }

    arr->arr[arr->len++] = node;

    unused_(p);
    return &arr->node;
}

struct hg_ast_node* hg_ast_for_stat_new(struct hg_parser* p,
                                        struct hg_ast_node* params,
                                        struct hg_ast_node* iterator,
                                        struct hg_ast_node* block) {
    assert(block->type == AST_NODE_ARRAY);
    assert(params->type == AST_NODE_ARRAY);

    _new(for_stat, struct hg_ast_for_stat);

    for_stat->node.type = AST_NODE_FOR;
    for_stat->node.line = params->line;
    for_stat->params    = _raw(params, struct hg_ast_node_array);
    for_stat->block     = _raw(block, struct hg_ast_node_array);
    for_stat->iterator  = iterator;

    unused_(p);
    return &for_stat->node;
}

struct hg_ast_node* hg_ast_break_stat_new(struct hg_parser* p) {
    _new(stat, struct hg_ast_break_stat);

    stat->node.type = AST_NODE_BREAK;
    stat->node.line = p->lineno;

    return &stat->node;
}

struct hg_ast_node* hg_ast_continue_stat_new(struct hg_parser* p) {
    _new(stat, struct hg_ast_continue_stat);

    stat->node.type = AST_NODE_CONTINUE;
    stat->node.line = p->lineno;

    return &stat->node;
}

struct hg_ast_node* hg_ast_return_stat_new(struct hg_parser* p,
                                           struct hg_ast_node* exprs) {
    assert(exprs == NULL || exprs->type == AST_NODE_ARRAY);

    _new(stat, struct hg_ast_return_stat);

    stat->node.type = AST_NODE_RETURN;
    stat->node.line = exprs != NULL ? exprs->line : p->lineno;
    stat->exprs     = _raw(exprs, struct hg_ast_node_array);

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_if_stat_new(struct hg_parser* p,
                                       struct hg_ast_node* cond,
                                       struct hg_ast_node* block,
                                       struct hg_ast_node* else_block) {
    assert(block->type == AST_NODE_ARRAY);
    assert(else_block == NULL || else_block->type == AST_NODE_ARRAY);

    _new(stat, struct hg_ast_if_stat);

    stat->node.type = AST_NODE_IF;
    stat->node.line = cond->line;
    stat->condition = cond;
    stat->block     = _raw(block, struct hg_ast_node_array);
    stat->else_block =
        else_block == NULL ? NULL : _raw(else_block, struct hg_ast_node_array);

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_assignment_stat_new(struct hg_parser* p,
                                               struct hg_ast_node* vars,
                                               struct hg_ast_node* exprs) {
    assert(vars->type == AST_NODE_ARRAY);
    assert(exprs->type == AST_NODE_ARRAY);

    _new(stat, struct hg_ast_assignment_stat);

    stat->node.type = AST_NODE_ASSIGNMENT;
    stat->node.line = vars->line;
    stat->vars      = _raw(vars, struct hg_ast_node_array);
    stat->exprs     = _raw(exprs, struct hg_ast_node_array);

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_func_stat_new(struct hg_parser* p,
                                         struct hg_ast_node* id,
                                         struct hg_ast_node* params,
                                         struct hg_ast_node* block) {
    assert(params == NULL || params->type == AST_NODE_ARRAY);
    assert(block->type == AST_NODE_ARRAY);
    assert(id->type == AST_NODE_LITERAL_ID);

    _new(stat, struct hg_ast_func_stat);

    stat->node.type = AST_NODE_FUNC;
    stat->node.line = id->line;
    stat->id        = id;
    stat->params    = _raw(params, struct hg_ast_node_array);
    stat->block     = _raw(block, struct hg_ast_node_array);

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_func_def_new(struct hg_parser* p,
                                        struct hg_ast_node* params,
                                        struct hg_ast_node* block) {
    assert(params == NULL || params->type == AST_NODE_ARRAY);
    assert(block->type == AST_NODE_ARRAY);

    _new(expr, struct hg_ast_func_def_expr);

    expr->node.type = AST_NODE_FUNC_DEF;
    expr->node.line = params != NULL ? params->line : block->line;
    expr->params    = _raw(params, struct hg_ast_node_array);
    expr->block     = _raw(block, struct hg_ast_node_array);

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_unary_expr_new(struct hg_parser* p,
                                          enum ast_node_op op,
                                          struct hg_ast_node* expr) {
    _new(unary, struct hg_ast_unary_expr);

    unary->node.type = AST_NODE_UNARY_EXPR;
    unary->node.line = expr->line;
    unary->expr      = expr;
    unary->op        = op;

    unused_(p);
    return &unary->node;
}

struct hg_ast_node* hg_ast_binary_expr_new(struct hg_parser* p,
                                           enum ast_node_op op,
                                           struct hg_ast_node* left,
                                           struct hg_ast_node* right) {
    _new(expr, struct hg_ast_binary_expr);

    expr->node.type = AST_NODE_BINARY_EXPR;
    expr->node.line = left->line;
    expr->left      = left;
    expr->right     = right;
    expr->op        = op;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_call_expr_new(struct hg_parser* p,
                                         struct hg_ast_node* callable,
                                         struct hg_ast_node* args) {
    assert(args == NULL || args->type == AST_NODE_ARRAY);

    _new(expr, struct hg_ast_call_expr);

    expr->node.type = AST_NODE_CALL;
    expr->node.line = callable->line;
    expr->callable  = callable;
    expr->args      = _raw(args, struct hg_ast_node_array);

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_field_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* prefix,
                                          struct hg_ast_node* field) {
    assert(field->type == AST_NODE_LITERAL_ID);

    _new(expr, struct hg_ast_field_expr);

    expr->node.type = AST_NODE_FIELD;
    expr->node.line = prefix->line;
    expr->prefix    = prefix;
    expr->field     = field;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_index_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* prefix,
                                          struct hg_ast_node* index) {
    _new(expr, struct hg_ast_index_expr);

    expr->node.type = AST_NODE_INDEX;
    expr->node.line = prefix->line;
    expr->prefix    = prefix;
    expr->index     = index;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_table_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* arr) {
    assert(arr == NULL || arr->type == AST_NODE_ARRAY);

    _new(expr, struct hg_ast_table_expr);

    expr->node.type = AST_NODE_TABLE;
    expr->node.line = arr != NULL ? arr->line : p->lineno;
    expr->entries   = _raw(arr, struct hg_ast_node_array);

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_table_entry_new(struct hg_parser* p,
                                           struct hg_ast_node* key,
                                           struct hg_ast_node* value) {
    _new(entry, struct hg_ast_table_entry);

    entry->node.type = AST_NODE_TABLE_ENTRY;
    entry->node.line = key != NULL ? key->line : value->line;
    entry->key       = key;
    entry->value     = value;

    unused_(p);
    return &entry->node;
}

struct hg_ast_node* hg_ast_literal_id_new(struct hg_parser* p, hg_char str) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = AST_NODE_LITERAL_ID;
    lit->node.line = p->lineno;
    lit->as_id     = str;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_str_new(struct hg_parser* p, hg_char str) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = AST_NODE_LITERAL_STR;
    lit->node.line = p->lineno;
    lit->as_str    = str;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_float_new(struct hg_parser* p, hg_float d) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = AST_NODE_LITERAL_FLOAT;
    lit->node.line = p->lineno;
    lit->as_float  = d;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_int_new(struct hg_parser* p, hg_int i) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = AST_NODE_LITERAL_INT;
    lit->node.line = p->lineno;
    lit->as_int    = i;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_bool_new(struct hg_parser* p, hg_bool b) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = AST_NODE_LITERAL_BOOL;
    lit->node.line = p->lineno;
    lit->as_bool   = b;

    return &lit->node;
}

#undef _new
#undef _raw
#undef _raw_to
