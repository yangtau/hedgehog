#include "ast.h"
#include "memory.h"
#include "string.h"

// TODO: add expr type for `(a<2 && (a<2 || b <1))`

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

#define _array_check(t)                                                     \
    do {                                                                    \
        assert(t > _HG_AST_NODE_ARRAY_START && t < _HG_AST_NODE_ARRAY_END); \
    } while (0)
#define _stat_check(t)                                                    \
    do {                                                                  \
        assert(t > _HG_AST_NODE_STAT_START && t < _HG_AST_NODE_STAT_END); \
    } while (0)
#define _expr_check(t)                                                    \
    do {                                                                  \
        assert(t > _HG_AST_NODE_EXPR_START && t < _HG_AST_NODE_EXPR_END); \
    } while (0)
#define _literal_check(t)                        \
    do {                                         \
        assert(t > _HG_AST_NODE_LITERAL_START && \
               t < _HG_AST_NODE_LITERAL_END);    \
    } while (0)

struct hg_ast_node* hg_ast_node_array_new(struct hg_parser* p,
                                          enum hg_ast_node_type type) {

    _array_check(type);
    struct hg_ast_node_array* arr =
        hg_alloc(sizeof(struct hg_ast_node_array) +
                 sizeof(struct hg_ast_node*) * _start_array_cap);

    arr->node.type = type;
    arr->node.line = p->lineno;
    arr->cap       = _start_array_cap;
    arr->len       = 0;

    return &arr->node;
}

struct hg_ast_node* hg_ast_node_array_append(struct hg_parser* p,
                                             struct hg_ast_node* _arr,
                                             struct hg_ast_node* node) {
    _array_check(_arr->type);
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
    assert(block->type == HG_AST_NODE_ARRAY_STATS);
    assert(params->type == HG_AST_NODE_ARRAY_PARAMS);

    _new(for_stat, struct hg_ast_for_stat);

    for_stat->node.type = HG_AST_NODE_FOR;
    for_stat->node.line = params->line;
    for_stat->params    = params;
    for_stat->block     = block;
    for_stat->iterator  = iterator;

    unused_(p);
    return &for_stat->node;
}

struct hg_ast_node* hg_ast_break_stat_new(struct hg_parser* p) {
    _new(stat, struct hg_ast_break_stat);

    stat->node.type = HG_AST_NODE_BREAK;
    stat->node.line = p->lineno;

    return &stat->node;
}

struct hg_ast_node* hg_ast_continue_stat_new(struct hg_parser* p) {
    _new(stat, struct hg_ast_continue_stat);

    stat->node.type = HG_AST_NODE_CONTINUE;
    stat->node.line = p->lineno;

    return &stat->node;
}

struct hg_ast_node* hg_ast_return_stat_new(struct hg_parser* p,
                                           struct hg_ast_node* exprs) {
    assert(exprs == NULL || exprs->type == HG_AST_NODE_ARRAY_EXPRS);

    _new(stat, struct hg_ast_return_stat);

    stat->node.type = HG_AST_NODE_RETURN;
    stat->node.line = exprs != NULL ? exprs->line : p->lineno;
    stat->exprs     = exprs;

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_if_stat_new(struct hg_parser* p,
                                       struct hg_ast_node* cond,
                                       struct hg_ast_node* block,
                                       struct hg_ast_node* else_block) {
    assert(block->type == HG_AST_NODE_ARRAY_STATS);
    assert(else_block == NULL || else_block->type == HG_AST_NODE_ARRAY_STATS);

    _new(stat, struct hg_ast_if_stat);

    stat->node.type  = HG_AST_NODE_IF;
    stat->node.line  = cond->line;
    stat->condition  = cond;
    stat->block      = block;
    stat->else_block = else_block == NULL ? NULL : else_block;

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_assignment_stat_new(struct hg_parser* p,
                                               struct hg_ast_node* vars,
                                               struct hg_ast_node* exprs) {
    assert(vars->type == HG_AST_NODE_ARRAY_VARS);
    assert(exprs->type == HG_AST_NODE_ARRAY_EXPRS);

    _new(stat, struct hg_ast_assignment_stat);

    stat->node.type = HG_AST_NODE_ASSIGNMENT;
    stat->node.line = vars->line;
    stat->vars      = vars;
    stat->exprs     = exprs;

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_func_stat_new(struct hg_parser* p,
                                         struct hg_ast_node* id,
                                         struct hg_ast_node* params,
                                         struct hg_ast_node* block) {
    assert(params == NULL || params->type == HG_AST_NODE_ARRAY_PARAMS);
    assert(block->type == HG_AST_NODE_ARRAY_STATS);
    assert(id->type == HG_AST_NODE_LITERAL_ID);

    _new(stat, struct hg_ast_func_stat);

    stat->node.type = HG_AST_NODE_FUNC;
    stat->node.line = id->line;
    stat->id        = id;
    stat->params    = params;
    stat->block     = block;

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_expr_stat_new(struct hg_parser* p,
                                         struct hg_ast_node* expr) {
    _new(stat, struct hg_ast_expr_stat);

    stat->node.type = HG_AST_NODE_EXPR_STAT;
    stat->node.line = expr->line;
    stat->expr      = expr;

    unused_(p);
    return &stat->node;
}

struct hg_ast_node* hg_ast_func_def_new(struct hg_parser* p,
                                        struct hg_ast_node* params,
                                        struct hg_ast_node* block) {
    assert(params == NULL || params->type == HG_AST_NODE_ARRAY_PARAMS);
    assert(block->type == HG_AST_NODE_ARRAY_STATS);

    _new(expr, struct hg_ast_func_def_expr);

    expr->node.type = HG_AST_NODE_FUNC_DEF;
    expr->node.line = params != NULL ? params->line : block->line;
    expr->params    = params;
    expr->block     = block;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_unary_expr_new(struct hg_parser* p,
                                          enum hg_ast_node_op op,
                                          struct hg_ast_node* expr) {
    _new(unary, struct hg_ast_unary_expr);

    unary->node.type = HG_AST_NODE_UNARY_EXPR;
    unary->node.line = expr->line;
    unary->expr      = expr;
    unary->op        = op;

    unused_(p);
    return &unary->node;
}

struct hg_ast_node* hg_ast_binary_expr_new(struct hg_parser* p,
                                           enum hg_ast_node_op op,
                                           struct hg_ast_node* left,
                                           struct hg_ast_node* right) {
    _new(expr, struct hg_ast_binary_expr);

    expr->node.type = HG_AST_NODE_BINARY_EXPR;
    expr->node.line = left->line;
    expr->left      = left;
    expr->right     = right;
    expr->op        = op;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_call_expr_new(struct hg_parser* p,
                                         struct hg_ast_node* callable,
                                         struct hg_ast_node* exprs) {
    assert(exprs == NULL || exprs->type == HG_AST_NODE_ARRAY_EXPRS);

    _new(expr, struct hg_ast_call_expr);

    expr->node.type = HG_AST_NODE_CALL;
    expr->node.line = callable->line;
    expr->callable  = callable;
    expr->exprs     = exprs;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_field_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* prefix,
                                          struct hg_ast_node* field) {
    assert(field->type == HG_AST_NODE_LITERAL_ID);

    _new(expr, struct hg_ast_field_expr);

    expr->node.type = HG_AST_NODE_FIELD;
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

    expr->node.type = HG_AST_NODE_INDEX;
    expr->node.line = prefix->line;
    expr->prefix    = prefix;
    expr->index     = index;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_table_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* arr) {
    assert(arr == NULL ||
           arr->type == HG_AST_NODE_ARRAY_TABLE_ENTRIES /* for maps */ ||
           arr->type == HG_AST_NODE_ARRAY_EXPRS /* for lists*/);

    _new(expr, struct hg_ast_table_expr);

    expr->node.type = HG_AST_NODE_TABLE;
    expr->node.line = arr != NULL ? arr->line : p->lineno;
    expr->entries   = arr;

    unused_(p);
    return &expr->node;
}

struct hg_ast_node* hg_ast_table_entry_new(struct hg_parser* p,
                                           struct hg_ast_node* key,
                                           struct hg_ast_node* value) {
    _new(entry, struct hg_ast_table_entry);

    entry->node.type = HG_AST_NODE_TABLE_ENTRY;
    entry->node.line = key != NULL ? key->line : value->line;
    entry->key       = key;
    entry->value     = value;

    unused_(p);
    return &entry->node;
}

struct hg_ast_node* hg_ast_literal_id_new(struct hg_parser* p, hg_str str) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = HG_AST_NODE_LITERAL_ID;
    lit->node.line = p->lineno;
    lit->as_id     = str;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_str_new(struct hg_parser* p, hg_str str) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = HG_AST_NODE_LITERAL_STR;
    lit->node.line = p->lineno;
    lit->as_str    = str;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_float_new(struct hg_parser* p, hg_float d) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = HG_AST_NODE_LITERAL_FLOAT;
    lit->node.line = p->lineno;
    lit->as_float  = d;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_int_new(struct hg_parser* p, hg_int i) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = HG_AST_NODE_LITERAL_INT;
    lit->node.line = p->lineno;
    lit->as_int    = i;

    return &lit->node;
}

struct hg_ast_node* hg_ast_literal_bool_new(struct hg_parser* p, hg_bool b) {
    _new(lit, struct hg_ast_literal);

    lit->node.type = HG_AST_NODE_LITERAL_BOOL;
    lit->node.line = p->lineno;
    lit->as_bool   = b;

    return &lit->node;
}

void hg_ast_node_free(struct hg_ast_node* _node) {
    // TODO: remove this to reduce bugs
    if (_node == NULL)
        return;

    switch (_node->type) {
    case HG_AST_NODE_ASSIGNMENT: {
        _raw_to(_node, struct hg_ast_assignment_stat, node);
        hg_ast_node_free(node->exprs);
        hg_ast_node_free(node->vars);
        hg_free(node);
    } break;
    case HG_AST_NODE_FOR: {
        _raw_to(_node, struct hg_ast_for_stat, node);
        hg_ast_node_free(node->params);
        hg_ast_node_free(node->iterator);
        hg_ast_node_free(node->block);
        hg_free(node);
    } break;
    case HG_AST_NODE_IF: {
        _raw_to(_node, struct hg_ast_if_stat, node);
        hg_ast_node_free(node->condition);
        hg_ast_node_free(node->block);
        hg_ast_node_free(node->else_block);
        hg_free(node);
    } break;
    case HG_AST_NODE_FUNC: {
        _raw_to(_node, struct hg_ast_func_stat, node);
        hg_ast_node_free(node->id);
        hg_ast_node_free(node->params);
        hg_ast_node_free(node->block);
        hg_free(node);
    } break;
    case HG_AST_NODE_BREAK: {
        _raw_to(_node, struct hg_ast_break_stat, node);
        hg_free(node);
    } break;
    case HG_AST_NODE_CONTINUE: {
        _raw_to(_node, struct hg_ast_continue_stat, node);
        hg_free(node);
    } break;
    case HG_AST_NODE_RETURN: {
        _raw_to(_node, struct hg_ast_return_stat, node);
        hg_ast_node_free(node->exprs);
        hg_free(node);
    } break;
    case HG_AST_NODE_EXPR_STAT: {
        _raw_to(_node, struct hg_ast_expr_stat, node);
        hg_ast_node_free(node->expr);
        hg_free(node);
    } break;
    case HG_AST_NODE_CALL: {
        _raw_to(_node, struct hg_ast_call_expr, node);
        hg_ast_node_free(node->callable);
        hg_ast_node_free(node->exprs);
        hg_free(node);
    } break;
    case HG_AST_NODE_FIELD: {
        _raw_to(_node, struct hg_ast_field_expr, node);
        hg_ast_node_free(node->prefix);
        hg_ast_node_free(node->field);
        hg_free(node);
    } break;
    case HG_AST_NODE_INDEX: {
        _raw_to(_node, struct hg_ast_index_expr, node);
        hg_ast_node_free(node->prefix);
        hg_ast_node_free(node->index);
        hg_free(node);
    } break;
    case HG_AST_NODE_FUNC_DEF: {
        _raw_to(_node, struct hg_ast_func_def_expr, node);
        hg_ast_node_free(node->params);
        hg_ast_node_free(node->block);
        hg_free(node);
    } break;
    case HG_AST_NODE_BINARY_EXPR: {
        _raw_to(_node, struct hg_ast_binary_expr, node);
        hg_ast_node_free(node->left);
        hg_ast_node_free(node->right);
        hg_free(node);
    } break;
    case HG_AST_NODE_UNARY_EXPR: {
        _raw_to(_node, struct hg_ast_unary_expr, node);
        hg_ast_node_free(node->expr);
        hg_free(node);
    } break;
    case HG_AST_NODE_TABLE: {
        _raw_to(_node, struct hg_ast_table_expr, node);
        hg_ast_node_free(node->entries);
        hg_free(node);
    } break;
    case HG_AST_NODE_TABLE_ENTRY: {
        _raw_to(_node, struct hg_ast_table_entry, node);
        hg_ast_node_free(node->key);
        hg_ast_node_free(node->value);
        hg_free(node);
    } break;
    case HG_AST_NODE_LITERAL_STR: {
        _raw_to(_node, struct hg_ast_literal, node);
        // TODO: free reference to str
        hg_free(node);
    } break;
    case HG_AST_NODE_LITERAL_ID: {
        _raw_to(_node, struct hg_ast_literal, node);
        // TODO: free reference to str
        hg_free(node);
    } break;
    case HG_AST_NODE_LITERAL_BOOL: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_free(node);
    } break;
    case HG_AST_NODE_LITERAL_INT: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_free(node);
    } break;
    case HG_AST_NODE_LITERAL_FLOAT: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_free(node);
    } break;
    case HG_AST_NODE_ARRAY_STATS:
    case HG_AST_NODE_ARRAY_EXPRS:
    case HG_AST_NODE_ARRAY_VARS:
    case HG_AST_NODE_ARRAY_PARAMS:
    case HG_AST_NODE_ARRAY_TABLE_ENTRIES: {
        _raw_to(_node, struct hg_ast_node_array, node);
        for (size_t i = 0; i < node->len; i++) {
            hg_free(node->arr[i]);
        }
        hg_free(node);
    } break;
    default:
        unreachable_();
    }
}

static void _node_to_str(struct hg_ast_node* node, uint32_t indent,
                         uint32_t depth, struct hg_strbuf* buffer);

hg_str hg_ast_node_to_str(struct hg_ast_node* node, uint32_t indent) {
    struct hg_strbuf buf;

    hg_strbuf_init(&buf);

    _node_to_str(node, indent, 0, &buf);

    hg_str ret = hg_strbuf_to_str(&buf);
    hg_strbuf_free(&buf);
    return ret;
}

static hg_str _get_indent_str(uint32_t indent, uint32_t depth) {
    hg_str indent_str;

    struct hg_strbuf indent_buf;
    hg_strbuf_init(&indent_buf);

    for (size_t i = 0; i < indent * depth; i++)
        assert(hg_strbuf_append(&indent_buf, " "));
    indent_str = hg_strbuf_to_str(&indent_buf);
    hg_strbuf_free(&indent_buf);
    return indent_str;
}

static const char* _op_to_str(enum hg_ast_node_op op) {
    assert(op < _HG_AST_NODE_OP_END && op > _HG_AST_NODE_OP_START);
    const char* const chars[] = {
        [HG_AST_NODE_OP_AND] = "and", // and
        [HG_AST_NODE_OP_OR]  = "or",  // or
        [HG_AST_NODE_OP_NOT] = "not", // not
        [HG_AST_NODE_OP_NEQ] = "!=",  // !=
        [HG_AST_NODE_OP_EQ]  = "==",  // ==
        [HG_AST_NODE_OP_GE]  = ">=",  // >=
        [HG_AST_NODE_OP_LE]  = "<=",  // <=
        [HG_AST_NODE_OP_GT]  = ">",   // >
        [HG_AST_NODE_OP_LS]  = "<",   // <
        [HG_AST_NODE_OP_ADD] = "+",   // +
        [HG_AST_NODE_OP_SUB] = "-",   // -
        [HG_AST_NODE_OP_MUL] = "*",   // *
        [HG_AST_NODE_OP_DIV] = "/",   // /
        [HG_AST_NODE_OP_MOD] = "%",   // %
    };

    return chars[op];
}

static void _node_to_str(struct hg_ast_node* _node, uint32_t indent,
                         uint32_t depth, struct hg_strbuf* buf) {
#define add_indent hg_strbuf_append(buf, _get_indent_str(indent, depth))
#define add_nl     hg_strbuf_append(buf, "\n")
#define add_space  hg_strbuf_append(buf, " ")
#define add_block(block, end)                                  \
    do {                                                       \
        if (_raw(block, struct hg_ast_node_array)->len == 0) { \
            hg_strbuf_append(buf, "{}%s", end);         \
        } else {                                               \
            hg_strbuf_append(buf, "{\n");               \
            _node_to_str(block, indent, depth + 1, buf);       \
            add_indent;                                        \
            hg_strbuf_append(buf, "}%s", end);          \
        }                                                      \
    } while (0)

    switch (_node->type) {
    case HG_AST_NODE_ASSIGNMENT: {
        _raw_to(_node, struct hg_ast_assignment_stat, node);

        add_indent;
        _node_to_str(node->vars, indent, depth, buf);
        hg_strbuf_append(buf, " = ");
        _node_to_str(node->exprs, indent, depth, buf);
        add_nl;
    } break;
    case HG_AST_NODE_FOR: {
        _raw_to(_node, struct hg_ast_for_stat, node);

        add_indent;
        hg_strbuf_append(buf, "for ");
        _node_to_str(node->params, indent, depth, buf);
        hg_strbuf_append(buf, " in ");
        _node_to_str(node->iterator, indent, depth, buf);

        add_space;
        add_block(node->block, "\n");
    } break;
    case HG_AST_NODE_IF: {
        _raw_to(_node, struct hg_ast_if_stat, node);

        add_indent;
        hg_strbuf_append(buf, "if ");
        _node_to_str(node->condition, indent, depth, buf);
        add_space;

        // if block
        add_block(node->block, node->else_block ? " " : "\n");

        // else block
        if (node->else_block) {
            hg_strbuf_append(buf, "else ");
            // TODO: do not inline this
            add_block(node->else_block, "\n");
        }
    } break;
    case HG_AST_NODE_FUNC: {
        _raw_to(_node, struct hg_ast_func_stat, node);

        add_indent;
        hg_strbuf_append(buf, "fn ");
        _node_to_str(node->id, indent, depth, buf);

        hg_strbuf_append(buf, "(");
        if (node->params) {
            _node_to_str(node->params, indent, depth, buf);
        }
        hg_strbuf_append(buf, ") ");

        add_block(node->block, "\n");
    } break;
    case HG_AST_NODE_BREAK: {
        add_indent;
        hg_strbuf_append(buf, "break\n");
    } break;
    case HG_AST_NODE_CONTINUE: {
        add_indent;
        hg_strbuf_append(buf, "continue\n");
    } break;
    case HG_AST_NODE_RETURN: {
        _raw_to(_node, struct hg_ast_return_stat, node);

        add_indent;
        hg_strbuf_append(buf, "return");

        if (node->exprs) {
            add_space;
            _node_to_str(node->exprs, indent, depth, buf);
        }
        add_nl;
    } break;
    case HG_AST_NODE_EXPR_STAT: {
        _raw_to(_node, struct hg_ast_expr_stat, node);
        add_indent;
        _node_to_str(node->expr, indent, depth, buf);
        add_nl;
    } break;
    // exprs (no indent and no new line by default):
    case HG_AST_NODE_CALL: {
        _raw_to(_node, struct hg_ast_call_expr, node);

        _node_to_str(node->callable, indent, depth, buf);

        hg_strbuf_append(buf, "(");
        if (node->exprs) {
            _node_to_str(node->exprs, indent, depth, buf);
        }
        hg_strbuf_append(buf, ")");
    } break;
    case HG_AST_NODE_FIELD: {
        _raw_to(_node, struct hg_ast_field_expr, node);

        _node_to_str(node->prefix, indent, depth, buf);
        hg_strbuf_append(buf, ".");
        _node_to_str(node->field, indent, depth, buf);
    } break;
    case HG_AST_NODE_INDEX: {
        _raw_to(_node, struct hg_ast_index_expr, node);

        _node_to_str(node->prefix, indent, depth, buf);

        hg_strbuf_append(buf, "[");
        _node_to_str(node->index, indent, depth, buf);
        hg_strbuf_append(buf, "]");
    } break;
    case HG_AST_NODE_FUNC_DEF: {
        _raw_to(_node, struct hg_ast_func_def_expr, node);

        hg_strbuf_append(buf, "fn ");

        hg_strbuf_append(buf, "(");
        if (node->params) {
            _node_to_str(node->params, indent, depth, buf);
        }
        hg_strbuf_append(buf, ") ");

        add_block(node->block, "");
    } break;
    case HG_AST_NODE_BINARY_EXPR: {
        _raw_to(_node, struct hg_ast_binary_expr, node);
        _node_to_str(node->left, indent, depth, buf);
        hg_strbuf_append(buf, " %s ", _op_to_str(node->op));
        _node_to_str(node->right, indent, depth, buf);
    } break;
    case HG_AST_NODE_UNARY_EXPR: {
        _raw_to(_node, struct hg_ast_unary_expr, node);
        hg_strbuf_append(buf, "%s ", _op_to_str(node->op));
        _node_to_str(node->expr, indent, depth, buf);
    } break;
    case HG_AST_NODE_TABLE: {
        _raw_to(_node, struct hg_ast_table_expr, node);
        hg_strbuf_append(buf, "{");
        if (node->entries) {
            _node_to_str(node->entries, indent, depth, buf);
        }
        hg_strbuf_append(buf, "}");
    } break;
    case HG_AST_NODE_TABLE_ENTRY: {
        _raw_to(_node, struct hg_ast_table_entry, node);
        _node_to_str(node->key, indent, depth, buf);
        hg_strbuf_append(buf, ": ");
        _node_to_str(node->value, indent, depth, buf);
    } break;
    case HG_AST_NODE_LITERAL_STR: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_strbuf_append(buf, "\"%s\"", node->as_str);
    } break;
    case HG_AST_NODE_LITERAL_ID: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_strbuf_append(buf, "%s", node->as_id);
    } break;
    case HG_AST_NODE_LITERAL_BOOL: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_strbuf_append(buf, "%s", node->as_bool ? "true" : "false");
    } break;
    case HG_AST_NODE_LITERAL_INT: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_strbuf_append(buf, "%" HGFormatInt, node->as_int);
    } break;
    case HG_AST_NODE_LITERAL_FLOAT: {
        _raw_to(_node, struct hg_ast_literal, node);
        hg_strbuf_append(buf, "%" HGFormatFloat, node->as_float);
    } break;
    case HG_AST_NODE_ARRAY_STATS: {
        _raw_to(_node, struct hg_ast_node_array, node);
        for (size_t i = 0; i < node->len; i++) {
            _node_to_str(node->arr[i], indent, depth, buf);
            // add_nl;
        }
    } break;
    case HG_AST_NODE_ARRAY_EXPRS:
    case HG_AST_NODE_ARRAY_VARS:
    case HG_AST_NODE_ARRAY_PARAMS:
    case HG_AST_NODE_ARRAY_TABLE_ENTRIES: {
        _raw_to(_node, struct hg_ast_node_array, node);
        for (size_t i = 0; i < node->len; i++) {
            _node_to_str(node->arr[i], indent, depth, buf);
            if (i != node->len - 1) {
                hg_strbuf_append(buf, ", ");
            }
        }
    } break;
        break;
    default:
        unreachable_();
    }
#undef add_indent
#undef add_nl
#undef add_space
#undef add_block
}

#undef _new
#undef _raw
#undef _raw_to
#undef _array_check
#undef _expr_check
#undef _stat_check
#undef _literal_check
