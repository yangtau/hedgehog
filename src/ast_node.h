#ifndef _HG_AST_NODE_H_
#define _HG_AST_NODE_H_
#include "common.h"

//> parser_state
struct parser_state {
    int nerr;
    void* lval;
    const char* fname;
    int lineno;
    int tline;
};
//< parser_state

//> ast_node
enum ast_node_type {
    AST_NODE_ASSIGN,
    AST_NODE_LET,
    AST_NODE_OP,
    AST_NODE_IF,
    AST_NODE_FOR,
    AST_NODE_CALL,
    AST_NODE_FUNC,
    AST_NODE_WHILE,
    AST_NODE_INDEX,
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
    AST_NODE_TUPLE,
    AST_NODE_MAP,
};

struct ast_node {
    enum ast_node_type type;
    void* node;
};
//< ast_node

//> ast_node_op
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
/* ast_node_op: a binary or unary expression, 
 * @left: `NULL` for unary expression
 */
struct ast_node_op {
    enum ast_node_op_type op;
    struct ast_node* left; // for unary expression, left == NULL
    struct ast_node* right;
};

struct ast_node* ast_node_op_new(struct parser_state* p,
                                 enum ast_node_op_type type,
                                 struct ast_node* left, struct ast_node* right);
//< ast_node_op

//>
struct ast_node_index {
    struct ast_node* xs;
    struct ast_node* idx;
};
struct ast_node* ast_node_index_new(struct parser_state* p, struct ast_node* xs,
                                    struct ast_node* idx);
//<

//> ast_node_if
struct ast_node_if {
    struct ast_node* cond; // for *else* branch, `cond` is NULL
    struct ast_node* stats;
    struct ast_node* opt_else; // `opt_else->type` is `AST_NODE_IF`
};

struct ast_node* ast_node_if_new(struct parser_state* p, struct ast_node* cond,
                                 struct ast_node* stats,
                                 struct ast_node* opt_else);
//< ast_node_if

//> ast_node_func
struct ast_node_func {
    struct ast_node* id;
    struct ast_node* vars; // can be NULL
    struct ast_node* stats;
};

struct ast_node* ast_node_func_new(struct parser_state* p, struct ast_node* id,
                                   struct ast_node* vars,
                                   struct ast_node* stats);
//< ast_node_func

//> ast_node_call
struct ast_node_call {
    struct ast_node* func;
    struct ast_node* args;
};

struct ast_node* ast_node_call_new(struct parser_state* p,
                                   struct ast_node* func,
                                   struct ast_node* args);
//< ast_node_call

//> ast_node_return
/* ast_node_return_new: ast_node->node = expr
 */
struct ast_node* ast_node_return_new(struct parser_state* p,
                                     struct ast_node* expr);
//< ast_node_return

//> ast_node_assign
struct ast_node_assign {
    struct ast_node* vars;
    struct ast_node* args;
};
struct ast_node* ast_node_assign_new(struct parser_state* p,
                                     struct ast_node* vars,
                                     struct ast_node* args);
//< ast_node_assign

//> ast_node_assign
struct ast_node_let {
    struct ast_node* vars;
    struct ast_node* args;
};
struct ast_node* ast_node_let_new(struct parser_state* p, struct ast_node* vars,
                                  struct ast_node* args);
//< ast_node_assign

//> ast_node_while
struct ast_node_while {
    struct ast_node* cond;
    struct ast_node* stats;
};
struct ast_node* ast_node_while_new(struct parser_state* p,
                                    struct ast_node* cond,
                                    struct ast_node* stats);
//< ast_node_while

//> ast_node_for
struct ast_node_for {
    struct ast_node* vars;
    struct ast_node* iterator;
    struct ast_node* stats;
};
struct ast_node* ast_node_for_new(struct parser_state* p, struct ast_node* vars,
                                  struct ast_node* iterator,
                                  struct ast_node* stats);
//< ast_node_for

//> ast_node_loopctrl
/* ast_node_loopctrl_new:
 * @type: AST_NODE_BREAK or AST_NODE_CONTINUE
 */
struct ast_node* ast_node_loopctrl_new(struct parser_state* p,
                                       enum ast_node_type type);
//< ast_node_loopctrl

//> ast_node_array
struct ast_node_array {
    size_t len;
    size_t capacity;
    struct ast_node* arr[];
};
/* ast_node_array_new: create a node array
 * @type: AST_NODE_ARGS, AST_NODE_VARS, AST_NODE_STATS, AST_NODE_LIST,
 *        AST_NODE_TUPLE
 */
struct ast_node* ast_node_array_new(struct parser_state* p,
                                    enum ast_node_type type);
/* ast_node_array_add: add `item` into `arr`
 */
void ast_node_array_add(struct parser_state* p, struct ast_node* arr,
                        struct ast_node* item);
struct ast_node* ast_node_list_new(struct parser_state* p,
                                   struct ast_node* args);
struct ast_node* ast_node_tuple_new(struct parser_state* p,
                                    struct ast_node* args);
struct ast_node* ast_node_map_new(struct parser_state* p,
                                    struct ast_node* args);                            
//< ast_node_array

//> ast_node_value: ast_node->node = hg_value_ptr;
struct ast_node* ast_node_int_new(struct parser_state* p, int64_t v);
struct ast_node* ast_node_float_new(struct parser_state* p, double v);
struct ast_node* ast_node_str_new(struct parser_state* p, const char* s);
struct ast_node* ast_node_str_len_new(struct parser_state* p, const char* s,
                                      size_t len);
struct ast_node* ast_node_bool_new(struct parser_state* p, bool v);
struct ast_node* ast_node_id_new(struct parser_state* p, const char* s);
//< ast_node_value

//> ast_node_free
void ast_node_free(struct ast_node* node);
//< ast_node_free

//> ast_node_dump
void ast_node_dump(struct ast_node* node, int indent, FILE* fp);
//< ast_node_dump
#endif // _HG_AST_NODE_H_
