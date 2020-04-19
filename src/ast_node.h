#ifndef _HG_AST_NODE_H_
#define _HG_AST_NODE_H_

struct parser_state {
    const char *fname;
    int lineno;
    int tline;
};

enum ast_node_type {
    AST_NODE_ASSIN,
    AST_NODE_OP,
    AST_NODE_IF,
    AST_NODE_FOR,
    AST_NODE_CALL,
    AST_NODE_FUNC,
    AST_NODE_RETURN,
    // type only node:
    AST_NODE_BREAK,
    AST_NODE_CONTINUE,
    // store as array:
    AST_NODE_STATS,
    AST_NODE_VARS,
    AST_NODE_ARGS,
    // store as hg_value:
    AST_NODE_VALUE,
};

struct ast_node {
    enum ast_node_type type;
    void *node;
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
    AST_NODE_OP_ADD,
    AST_NODE_OP_SUB,
    AST_NODE_OP_MUL,
    AST_NODE_OP_DIV,
    AST_NODE_OP_MOD,
};

struct ast_node_op {
    enum ast_node_op_type op;
    struct ast_node *left; // for unary expression, left == NULL
    struct ast_node *right;
};

struct ast_node_if {
    struct node *cond; // for *else* branch, `cond` is NULL
    struct node *stats;
    struct node *opt_else; // `opt_else->type` is `AST_NODE_IF`
};

struct ast_node_func {
    struct node *id;
    struct node *vars;
    struct node *stats;
};

struct ast_node_for {
    struct node *init;
    struct node *cond;
    struct node *update;
    struct node *stats;
};

struct ast_node_call {
    struct node *id;
    struct node *args;
};

struct ast_node_assign {
    struct node *vars;
    struct node *args;
};

struct ast_node_return {
    struct node *expr;
};

/** ast_node_op_new: create a binary or unary expression, 
 * @left: `NULL` for unary expression
 */
struct ast_node *ast_node_op_new(enum ast_node_op_type type,
                                 struct ast_node *left, struct ast_node *right);
/** ast_node_if_new: create a *if-else* node
 * @cond: `NULL` for *else*, not `NULL` for *else if* and *if*
 * @opt_else: `NULL` for *else*
 */
struct ast_node *ast_node_if_new(struct ast_node *cond, struct ast_node *stats,
                                 struct ast_node *opt_else);
/** ast_node_call_new: create a *call* node, `args` can be `NULL`
 */
struct ast_node *ast_node_call_new(struct ast_node *id, struct ast_node *args);
/** ast_node_func_new: create a *func* node, `vars` can be `NULL`
 */
struct ast_node *ast_node_func_new(struct ast_node *id, struct ast_node *vars,
                                   struct ast_node *stats);
/** ast_node_array_new: create a node array with the first item
 * @type: AST_NODE_ARGS, AST_NODE_VARS or AST_NODE_STATS
 * @first: the first node in the array
 */
struct ast_node *ast_node_array_new(enum ast_node_type type,
                                    struct ast_node *first);
/** ast_node_array_add: add `item` into `arr`
 */
void ast_node_array_add(struct ast_node *arr, struct ast_node *item);

/** ast_node_assign_new:
 */
struct ast_node *ast_node_assign_new(struct ast_node *vars,
                                     struct ast_node *args);
/** ast_node_return_new:
 */
struct ast_node *ast_node_return_new(struct ast_node *expr);

/** ast_node_for_new:
 * `for cond {}`: `init` and `update` are NULL
 * `for init; cond; update {}`:
 * `for {}`: `init`, `update` and `cond` are NULL
 */
struct ast_node *ast_node_for_new(struct ast_node *init, struct ast_node *cond,
                                  struct ast_node *update,
                                  struct ast_node *stats);
/** ast_node_loopctrl_new:
 * @type: AST_NODE_BREAK or AST_NODE_CONTINUE
 */
struct ast_node *ast_node_loopctrl_new(enum ast_node_type type);
#endif // _HG_AST_NODE_H_
