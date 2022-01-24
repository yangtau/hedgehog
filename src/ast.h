#ifndef HG_AST_H_
#define HG_AST_H_

#include "common.h"

struct hg_parser {
    const char* fname;
    int nerr;
    int lineno;
    int tline;
    struct hg_ast_node* ast;
};

// TODO: rename enums
enum hg_ast_node_type {
    // arrays:
    _HG_AST_NODE_ARRAY_START,
    HG_AST_NODE_ARRAY_STATS,
    HG_AST_NODE_ARRAY_EXPRS,
    HG_AST_NODE_ARRAY_VARS,
    HG_AST_NODE_ARRAY_PARAMS,
    HG_AST_NODE_ARRAY_TABLE_ENTRIES,
    _HG_AST_NODE_ARRAY_END,
    // statement:
    _HG_AST_NODE_STAT_START,
    HG_AST_NODE_ASSIGNMENT,
    HG_AST_NODE_FOR,
    HG_AST_NODE_IF,
    HG_AST_NODE_FUNC,
    HG_AST_NODE_BREAK,
    HG_AST_NODE_CONTINUE,
    HG_AST_NODE_RETURN,
    HG_AST_NODE_EXPR_STAT,
    _HG_AST_NODE_STAT_END,
    // exprs:
    _HG_AST_NODE_EXPR_START,
    HG_AST_NODE_CALL,
    HG_AST_NODE_FIELD,
    HG_AST_NODE_INDEX,
    HG_AST_NODE_FUNC_DEF,
    HG_AST_NODE_BINARY_EXPR,
    HG_AST_NODE_UNARY_EXPR,
    HG_AST_NODE_TABLE,
    HG_AST_NODE_TABLE_ENTRY,
    _HG_AST_NODE_EXPR_END,
    // literal:
    _HG_AST_NODE_LITERAL_START,
    HG_AST_NODE_LITERAL_STR,
    HG_AST_NODE_LITERAL_ID,
    HG_AST_NODE_LITERAL_BOOL,
    HG_AST_NODE_LITERAL_INT,
    HG_AST_NODE_LITERAL_FLOAT,
    _HG_AST_NODE_LITERAL_END,
};

enum hg_ast_node_op {
    _HG_AST_NODE_OP_START,
    HG_AST_NODE_OP_AND, // and
    HG_AST_NODE_OP_OR,  // or
    HG_AST_NODE_OP_NOT, // not
    HG_AST_NODE_OP_NEQ, // !=
    HG_AST_NODE_OP_EQ,  // ==
    HG_AST_NODE_OP_GE,  // >=
    HG_AST_NODE_OP_LE,  // <=
    HG_AST_NODE_OP_GT,  // >
    HG_AST_NODE_OP_LS,  // <
    HG_AST_NODE_OP_ADD, // +
    HG_AST_NODE_OP_SUB, // -
    HG_AST_NODE_OP_MUL, // *
    HG_AST_NODE_OP_DIV, // /
    HG_AST_NODE_OP_MOD, // %
    _HG_AST_NODE_OP_END,
};

struct hg_ast_node {
    enum hg_ast_node_type type;
    int line; // start line
};
#define _hg_ast_head struct hg_ast_node node

struct hg_ast_node_array {
    _hg_ast_head;
    size_t len, cap;
    struct hg_ast_node* arr[];
};
struct hg_ast_node* hg_ast_node_array_new(struct hg_parser* p,
                                          enum hg_ast_node_type type);
struct hg_ast_node* hg_ast_node_array_append(struct hg_parser* p,
                                             struct hg_ast_node* arr,
                                             struct hg_ast_node* node);

struct hg_ast_for_stat {
    _hg_ast_head;
    struct hg_ast_node* params;
    struct hg_ast_node* iterator;
    struct hg_ast_node* block;
};
struct hg_ast_node* hg_ast_for_stat_new(struct hg_parser* p,
                                        struct hg_ast_node* params,
                                        struct hg_ast_node* iterator,
                                        struct hg_ast_node* block);

struct hg_ast_break_stat {
    _hg_ast_head;
};
struct hg_ast_node* hg_ast_break_stat_new(struct hg_parser* p);

struct hg_ast_continue_stat {
    _hg_ast_head;
};
struct hg_ast_node* hg_ast_continue_stat_new(struct hg_parser* p);

struct hg_ast_return_stat {
    _hg_ast_head;
    struct hg_ast_node* exprs;
};
struct hg_ast_node* hg_ast_return_stat_new(struct hg_parser* p,
                                           struct hg_ast_node* exprs);

struct hg_ast_if_stat {
    _hg_ast_head;
    struct hg_ast_node* condition;
    struct hg_ast_node* block;
    struct hg_ast_node* else_block;
};
struct hg_ast_node* hg_ast_if_stat_new(struct hg_parser* p,
                                       struct hg_ast_node* cond,
                                       struct hg_ast_node* block,
                                       struct hg_ast_node* else_block);

struct hg_ast_assignment_stat {
    _hg_ast_head;
    struct hg_ast_node* vars;
    struct hg_ast_node* exprs;
};
struct hg_ast_node* hg_ast_assignment_stat_new(struct hg_parser* p,
                                               struct hg_ast_node* vars,
                                               struct hg_ast_node* exprs);

struct hg_ast_func_stat {
    _hg_ast_head;
    struct hg_ast_node* id;
    struct hg_ast_node* params;
    struct hg_ast_node* block;
};
struct hg_ast_node* hg_ast_func_stat_new(struct hg_parser* p,
                                         struct hg_ast_node* id,
                                         struct hg_ast_node* params,
                                         struct hg_ast_node* block);

struct hg_ast_expr_stat {
    _hg_ast_head;
    struct hg_ast_node* expr;
};
struct hg_ast_node* hg_ast_expr_stat_new(struct hg_parser* p,
                                         struct hg_ast_node* expr);

struct hg_ast_func_def_expr {
    _hg_ast_head;
    struct hg_ast_node* params;
    struct hg_ast_node* block;
};
struct hg_ast_node* hg_ast_func_def_new(struct hg_parser* p,
                                        struct hg_ast_node* params,
                                        struct hg_ast_node* block);

struct hg_ast_unary_expr {
    _hg_ast_head;
    enum hg_ast_node_op op;
    struct hg_ast_node* expr;
};
struct hg_ast_node* hg_ast_unary_expr_new(struct hg_parser* p,
                                          enum hg_ast_node_op op,
                                          struct hg_ast_node* expr);

struct hg_ast_binary_expr {
    _hg_ast_head;
    enum hg_ast_node_op op;
    struct hg_ast_node* left;
    struct hg_ast_node* right;
};
struct hg_ast_node* hg_ast_binary_expr_new(struct hg_parser* p,
                                           enum hg_ast_node_op op,
                                           struct hg_ast_node* left,
                                           struct hg_ast_node* right);

struct hg_ast_call_expr {
    _hg_ast_head;
    struct hg_ast_node* callable;
    struct hg_ast_node* exprs; // args
};
struct hg_ast_node* hg_ast_call_expr_new(struct hg_parser* p,
                                         struct hg_ast_node* callable,
                                         struct hg_ast_node* exprs);

struct hg_ast_field_expr {
    _hg_ast_head;
    struct hg_ast_node* prefix;
    struct hg_ast_node* field;
};
struct hg_ast_node* hg_ast_field_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* prefix,
                                          struct hg_ast_node* field);

struct hg_ast_index_expr {
    _hg_ast_head;
    struct hg_ast_node* prefix;
    struct hg_ast_node* index;
};
struct hg_ast_node* hg_ast_index_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* prefix,
                                          struct hg_ast_node* index);

struct hg_ast_table_expr {
    _hg_ast_head;
    struct hg_ast_node* entries;
};

struct hg_ast_node* hg_ast_table_expr_new(struct hg_parser* p,
                                          struct hg_ast_node* arr);

struct hg_ast_table_entry {
    _hg_ast_head;
    struct hg_ast_node* key;
    struct hg_ast_node* value;
};
struct hg_ast_node* hg_ast_table_entry_new(struct hg_parser* p,
                                           struct hg_ast_node* key,
                                           struct hg_ast_node* value);

struct hg_ast_literal {
    _hg_ast_head;
    union {
        hg_str as_id;
        hg_str as_str;
        hg_float as_float;
        hg_int as_int;
        hg_bool as_bool;
    };
};
struct hg_ast_node* hg_ast_literal_id_new(struct hg_parser* p, hg_str str);
struct hg_ast_node* hg_ast_literal_str_new(struct hg_parser* p, hg_str str);
struct hg_ast_node* hg_ast_literal_float_new(struct hg_parser* p, hg_float d);
struct hg_ast_node* hg_ast_literal_int_new(struct hg_parser* p, hg_int i);
struct hg_ast_node* hg_ast_literal_bool_new(struct hg_parser* p, hg_bool b);

#undef _hg_ast_head

hg_str hg_ast_node_to_str(struct hg_ast_node* node, uint32_t indent);
void hg_ast_node_free(struct hg_ast_node* node);

#endif // HG_AST_H_
