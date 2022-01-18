#ifndef HG_AST_H_
#define HG_AST_H_

#include "common.h"

struct hg_parser_state {
    int nerr;
    void* lval;
    const char* fname;
    int lineno;
    int tline;
};

enum hg_ast_node_type {
    // statement
    AST_NODE_ASSIGNMENT,
    AST_NODE_FOR,
    AST_NODE_IF,
    AST_NODE_FUNC,

    AST_NODE_CALL,
    AST_NODE_WHILE,
    AST_NODE_INDEX,
    AST_NODE_DOT,
    // only node:
    AST_NODE_BREAK,    // node->node = NULL
    AST_NODE_CONTINUE, // node->node = NULL
    AST_NODE_VALUE,    // node->node = hg_value
    AST_NODE_RETURN,   // node->node = expr
    // store as array:
    AST_NODE_STATS, // statement array
    AST_NODE_VARS,
    AST_NODE_ARGS,
    AST_NODE_LIST,
    AST_NODE_MAP,
};

enum ast_node_op_type {
    AST_NODE_OP_AND,
    AST_NODE_OP_OR,
    AST_NODE_OP_NOT,
    AST_NODE_OP_NEQ,
    AST_NODE_OP_EQ,
    AST_NODE_OP_GE,
    AST_NODE_OP_LE,
    AST_NODE_OP_GT,
    AST_NODE_OP_LS,
    AST_NODE_OP_NEG,
    AST_NODE_OP_ADD,
    AST_NODE_OP_SUB,
    AST_NODE_OP_MUL,
    AST_NODE_OP_DIV,
    AST_NODE_OP_MOD,
    AST_NODE_OP_DDOT,
};

struct hg_ast_block {};

struct hg_ast_for_stat {};

struct hg_ast_if_stat {

    struct {
    } else_stat;
};

struct hg_ast_assignment_stat {};

struct hg_ast_func_stat {};

struct hg_ast_func_def_expr {};

#endif // HG_AST_H_
