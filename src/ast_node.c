#include "ast_node.h"
#include "value.h"
#include "memory.h"
#include "string.h"

#define assert_expr_(node)                                       \
    {                                                            \
        enum ast_node_type type = (node)->type;                  \
        assert(type == AST_NODE_VALUE || type == AST_NODE_OP ||  \
               type == AST_NODE_CALL || type == AST_NODE_LIST || \
               type == AST_NODE_TUPLE || type == AST_NODE_MAP || \
               type == AST_NODE_INDEX);                          \
    }

#define parse_error_(p, ...)                                             \
    do {                                                                 \
        struct parser_state* __p = (p);                                  \
        __p->nerr++;                                                     \
        fprintf(stderr, "%s:%d:parser error:", __p->fname, __p->lineno); \
        fprintf(stderr, __VA_ARGS__);                                    \
        fprintf(stderr, "\n");                                           \
    } while (0)

//> ast_node
/* ast_node_new: malloc a new ast_node and set its type
 */
static inline struct ast_node* ast_node_new(enum ast_node_type type) {
    struct ast_node* node = hg_alloc_(struct ast_node);

    node->node = NULL;
    node->type = type;
    return node;
}
//< ast_node

//> ast_node_op
struct ast_node* ast_node_op_new(struct parser_state* p,
                                 enum ast_node_op_type type,
                                 struct ast_node* left,
                                 struct ast_node* right) {
    if (left == NULL)
        assert(type == AST_NODE_OP_NEG || type == AST_NODE_OP_NOT);
    else
        assert_expr_(left);
    assert_expr_(right);

    struct ast_node* node       = ast_node_new(AST_NODE_OP);
    struct ast_node_op* node_op = hg_alloc_(struct ast_node_op);

    node_op->op    = type;
    node_op->left  = left;
    node_op->right = right;

    node->node = node_op;
    return node;
}
//< ast_node_op

//> ast_node_index
struct ast_node* ast_node_index_new(struct parser_state* p, struct ast_node* xs,
                                    struct ast_node* idx) {
    struct ast_node* node        = ast_node_new(AST_NODE_INDEX);
    struct ast_node_index* index = hg_alloc_(struct ast_node_index);

    index->xs  = xs;
    index->idx = idx;

    node->node = index;
    return node;
}
//< ast_node_index

//> ast_node_if
struct ast_node* ast_node_if_new(struct parser_state* p, struct ast_node* cond,
                                 struct ast_node* stats,
                                 struct ast_node* opt_else) {
    assert(opt_else == NULL || opt_else->type == AST_NODE_IF);
    if (cond != NULL)
        assert_expr_(cond);

    struct ast_node* node       = ast_node_new(AST_NODE_IF);
    struct ast_node_if* node_if = hg_alloc_(struct ast_node_if);

    node_if->cond     = cond;
    node_if->stats    = stats;
    node_if->opt_else = opt_else;

    node->node = node_if;
    return node;
}
//< ast_node_if

//> ast_node_call
struct ast_node* ast_node_call_new(struct parser_state* p,
                                   struct ast_node* func,
                                   struct ast_node* args) {
    assert_expr_(func);
    assert(args == NULL || args->type == AST_NODE_ARGS);

    struct ast_node* node           = ast_node_new(AST_NODE_CALL);
    struct ast_node_call* node_call = hg_alloc_(struct ast_node_call);

    node_call->func = func;
    node_call->args = args;

    node->node = node_call;
    return node;
}
//< ast_node_call

//> ast_node_func
struct ast_node* ast_node_func_new(struct parser_state* p, struct ast_node* id,
                                   struct ast_node* vars,
                                   struct ast_node* stats) {
    assert(id->type == AST_NODE_VALUE);
    assert(vars == NULL || vars->type == AST_NODE_VARS);
    assert(stats == NULL || stats->type == AST_NODE_STATS);

    struct ast_node* node           = ast_node_new(AST_NODE_FUNC);
    struct ast_node_func* node_func = hg_alloc_(struct ast_node_func);

    node_func->id    = id;
    node_func->stats = stats;
    node_func->vars  = vars;

    node->node = node_func;
    return node;
}
//< ast_node_func

//> ast_node_array
struct ast_node* ast_node_array_new(struct parser_state* p,
                                    enum ast_node_type type) {
    struct ast_node* node = ast_node_new(type);
    struct ast_node_array* arr =
        flexible_alloc_(struct ast_node_array, struct ast_node*, 1);

    arr->capacity = 1;
    arr->len      = 0;

    node->node = arr;
    return node;
}

void ast_node_array_add(struct parser_state* p, struct ast_node* arr,
                        struct ast_node* item) {
    struct ast_node_array* node_arr = arr->node;
    assert(node_arr->capacity >= node_arr->len);

    if (node_arr->capacity == node_arr->len) {
        size_t cap         = node_arr->capacity;
        node_arr           = flexible_realloc_(node_arr, struct ast_node_array,
                                     struct ast_node*, cap, cap * 2);
        node_arr->capacity = cap * 2;
        arr->node          = node_arr; // update node ptr in ast_node
    }

    node_arr->arr[node_arr->len++] = item;
}

// TODO: Create a constant value if all items in a collection are constant values
//       - [ ] List
//       - [ ] Tuple
//       - [ ] Map
struct ast_node* ast_node_list_new(struct parser_state* p,
                                   struct ast_node* args) {
    assert(args == NULL || args->type == AST_NODE_ARGS);

    if (args == NULL)
        args = ast_node_array_new(p, AST_NODE_LIST);

    args->type = AST_NODE_LIST;
    return args;
}

struct ast_node* ast_node_tuple_new(struct parser_state* p,
                                    struct ast_node* args) {
    assert(args == NULL || args->type == AST_NODE_ARGS);

    if (args == NULL)
        args = ast_node_array_new(p, AST_NODE_TUPLE);

    args->type = AST_NODE_TUPLE;
    return args;
}

struct ast_node* ast_node_map_new(struct parser_state* p,
                                  struct ast_node* args) {
    assert(args != NULL);

    args->type = AST_NODE_MAP;
    return args;
}
//< ast_node_array

//> ast_node_assign
struct ast_node* ast_node_assign_new(struct parser_state* p,
                                     struct ast_node* vars,
                                     struct ast_node* args) {
    assert(vars->type == AST_NODE_VARS);
    assert(args->type == AST_NODE_ARGS);

    struct ast_node_array* _vars = vars->node;
    struct ast_node_array* _args = args->node;

    if (_vars->len != _args->len) {
        parse_error_(p, "assignment: expect %lu argument but given %lu",
                     _vars->len, _args->len);
    }

    struct ast_node* node               = ast_node_new(AST_NODE_ASSIGN);
    struct ast_node_assign* node_assign = hg_alloc_(struct ast_node_assign);

    node_assign->vars = vars;
    node_assign->args = args;

    node->node = node_assign;
    return node;
}
//< ast_node_assign

//> ast_node_let
struct ast_node* ast_node_let_new(struct parser_state* p, struct ast_node* vars,
                                  struct ast_node* args) {
    assert(vars->type == AST_NODE_VARS);
    assert(args->type == AST_NODE_ARGS);

    struct ast_node_array* _vars = vars->node;
    struct ast_node_array* _args = args->node;

    if (_vars->len != _args->len) {
        parse_error_(p, "assignment: expect %lu argument but given %lu",
                     _vars->len, _args->len);
    }

    struct ast_node* node               = ast_node_new(AST_NODE_LET);
    struct ast_node_assign* node_assign = hg_alloc_(struct ast_node_assign);

    node_assign->vars = vars;
    node_assign->args = args;

    node->node = node_assign;
    return node;
}
//< ast_node_let

//> ast_node_return
struct ast_node* ast_node_return_new(struct parser_state* p,
                                     struct ast_node* expr) {
    if (expr != NULL)
        assert_expr_(expr);

    struct ast_node* node = ast_node_new(AST_NODE_RETURN);

    node->node = expr;
    return node;
}
//< ast_node_return

//> ast_node_while
struct ast_node* ast_node_while_new(struct parser_state* p,
                                    struct ast_node* cond,
                                    struct ast_node* stats) {
    assert_expr_(cond);
    assert(stats == NULL || stats->type == AST_NODE_STATS);

    struct ast_node* node             = ast_node_new(AST_NODE_WHILE);
    struct ast_node_while* node_while = hg_alloc_(struct ast_node_while);

    node_while->cond  = cond;
    node_while->stats = stats;

    node->node = node_while;
    return node;
}
//< ast_node_while

//> ast_node_for
struct ast_node* ast_node_for_new(struct parser_state* p, struct ast_node* vars,
                                  struct ast_node* iterator,
                                  struct ast_node* stats) {
    assert(vars->type == AST_NODE_VARS);
    assert_expr_(iterator);
    assert(stats->type == AST_NODE_STATS);

    struct ast_node* node         = ast_node_new(AST_NODE_FOR);
    struct ast_node_for* node_for = hg_alloc_(struct ast_node_for);

    node_for->vars     = vars;
    node_for->iterator = iterator;
    node_for->stats    = stats;

    node->node = node_for;
    return node;
}
//< ast_node_for

//> ast_node_loopctrl
struct ast_node* ast_node_loopctrl_new(struct parser_state* p,
                                       enum ast_node_type type) {
    assert(type == AST_NODE_CONTINUE || type == AST_NODE_BREAK);

    return ast_node_new(type);
}
//< ast_node_loopctrl

//> ast_node_value
struct ast_node* ast_node_int_new(struct parser_state* p, int64_t v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_INT(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_float_new(struct parser_state* p, double v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_FLOAT(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_bool_new(struct parser_state* p, bool v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_BOOL(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_str_new(struct parser_state* p, const char* s) {
    return ast_node_str_len_new(p, s, strlen(s));
}

struct ast_node* ast_node_str_len_new(struct parser_state* p, const char* s,
                                      size_t len) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_OBJ(hg_string_copy(s, len));

    node->node = value;
    return node;
}

struct ast_node* ast_node_id_new(struct parser_state* p, const char* s) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_OBJ(hg_symbol_copy(s, strlen(s)));

    node->node = value;
    return node;
}
//< ast_node_value

//> ast_node_free
void ast_node_free(struct ast_node* node) {
    if (node == NULL)
        return;
    switch (node->type) {
    case AST_NODE_INDEX: {
        struct ast_node_index* idx = node->node;
        ast_node_free(idx->idx);
        ast_node_free(idx->xs);
        hg_free_(struct ast_node_index, idx);
    } break;

    case AST_NODE_ASSIGN: {
        struct ast_node_assign* node_assign = node->node;
        ast_node_free(node_assign->args);
        ast_node_free(node_assign->vars);
        hg_free_(struct ast_node_assign, node_assign);
    } break;

    case AST_NODE_LET: {
        struct ast_node_let* node_let = node->node;
        ast_node_free(node_let->args);
        ast_node_free(node_let->vars);
        hg_free_(struct ast_node_assign, node_let);
    } break;

    case AST_NODE_OP: {
        struct ast_node_op* node_op = node->node;
        ast_node_free(node_op->left);
        ast_node_free(node_op->right);
        hg_free_(struct ast_node_op, node_op);
    } break;
    case AST_NODE_IF: {
        struct ast_node_if* node_if = node->node;
        ast_node_free(node_if->cond);
        ast_node_free(node_if->stats);
        ast_node_free(node_if->opt_else);
        hg_free_(struct ast_node_if, node_if);
    } break;
    case AST_NODE_FOR: {
        struct ast_node_for* node_for = node->node;
        ast_node_free(node_for->iterator);
        ast_node_free(node_for->stats);
        ast_node_free(node_for->vars);
        hg_free_(struct ast_node_for, node_for);
    } break;
    case AST_NODE_CALL: {
        struct ast_node_call* node_call = node->node;
        ast_node_free(node_call->func);
        ast_node_free(node_call->args);
        hg_free_(struct ast_node_call, node_call);
    } break;
    case AST_NODE_FUNC: {
        struct ast_node_func* node_func = node->node;
        ast_node_free(node_func->id);
        ast_node_free(node_func->stats);
        ast_node_free(node_func->vars);
        hg_free_(struct ast_node_func, node_func);
    } break;
    case AST_NODE_WHILE: {
        struct ast_node_while* node_while = node->node;
        ast_node_free(node_while->cond);
        ast_node_free(node_while->stats);
        hg_free_(struct ast_node_while, node_while);
    } break;
    case AST_NODE_BREAK:
    case AST_NODE_CONTINUE: {
        // node->node = NULL
    } break;
    case AST_NODE_VALUE: {
        // node->node = hg_value
        struct hg_value* value = node->node;
        hg_value_free(*value);
        hg_free_(struct hg_value, value);
    } break;
    case AST_NODE_RETURN: {
        // node->node = ast_node
        ast_node_free(node->node);
    } break;
    case AST_NODE_STATS:
    case AST_NODE_VARS:
    case AST_NODE_ARGS:
    case AST_NODE_LIST:
    case AST_NODE_MAP:
    case AST_NODE_TUPLE: {
        struct ast_node_array* node_arr = node->node;
        for (size_t i = 0; i < node_arr->len; i++) {
            ast_node_free(node_arr->arr[i]);
        }
        flexible_free_(node_arr, struct ast_node_array, struct ast_node*,
                       node_arr->capacity);
    } break;
    default:
        unimplemented_("type %x", node->type);
    }
    hg_free_(struct ast_node, node);
}
//< ast_node_free

//> ast_node_dump
void ast_node_dump(struct ast_node* node, int indent, FILE* fp) {
    if (node == NULL)
        return;
    char buf[1024];

    int len = 0;
    for (; len < indent * 2; len++) {
        buf[len] = ' ';
    }
    buf[len] = '\0';

    switch (node->type) {
    case AST_NODE_TUPLE: {
        fprintf(fp, "%s(", buf);
        struct ast_node_array* arr = node->node;
        for (size_t i = 0; i < arr->len; i++) {
            ast_node_dump(arr->arr[i], 0, fp);
            if (i != arr->len - 1)
                fprintf(fp, ", ");
        }
        fprintf(fp, ")");
    } break;

    case AST_NODE_LIST: {
        fprintf(fp, "%s[", buf);
        struct ast_node_array* arr = node->node;
        for (size_t i = 0; i < arr->len; i++) {
            ast_node_dump(arr->arr[i], 0, fp);
            if (i != arr->len - 1)
                fprintf(fp, ", ");
        }
        fprintf(fp, "]");
    } break;

    case AST_NODE_MAP: {
        fprintf(fp, "%s{", buf);
        struct ast_node_array* arr = node->node;
        for (size_t i = 0; i < arr->len; i += 2) {
            ast_node_dump(arr->arr[i], 0, fp);
            fprintf(fp, ": ");
            ast_node_dump(arr->arr[i + 1], 0, fp);
            if (i != arr->len - 2)
                fprintf(fp, ", ");
        }
        fprintf(fp, "}");
    } break;

    case AST_NODE_STATS: {
        struct ast_node_array* arr = node->node;
        for (size_t i = 0; i < arr->len; i++) {
            ast_node_dump(arr->arr[i], indent, fp);
            fprintf(fp, "\n");
        }
    } break;

    case AST_NODE_FUNC: {
        struct ast_node_func* func = node->node;
        fprintf(fp, "%sdef ", buf);
        ast_node_dump(func->id, 0, fp);
        fprintf(fp, "(");
        ast_node_dump(func->vars, 0, fp);
        fprintf(fp, ") {\n");
        ast_node_dump(func->stats, indent + 1, fp);
        fprintf(fp, "%s}", buf);
    } break;

    case AST_NODE_WHILE: {
        struct ast_node_while* whl = node->node;
        fprintf(fp, "%swhile ", buf);
        ast_node_dump(whl->cond, 0, fp);
        fprintf(fp, " {\n");
        ast_node_dump(whl->stats, indent + 1, fp);
        fprintf(fp, "%s}", buf);
    } break;

    case AST_NODE_ASSIGN: {
        struct ast_node_assign* asg = node->node;
        fprintf(fp, "%s", buf);
        ast_node_dump(asg->vars, 0, fp);
        fprintf(fp, " = ");
        ast_node_dump(asg->args, 0, fp);
    } break;

    case AST_NODE_LET: {
        struct ast_node_assign* asg = node->node;
        fprintf(fp, "%slet ", buf);
        ast_node_dump(asg->vars, 0, fp);
        fprintf(fp, " = ");
        ast_node_dump(asg->args, 0, fp);
    } break;

    case AST_NODE_IF: {
        struct ast_node_if* nif = node->node;
        fprintf(fp, "%sif ", buf);
        ast_node_dump(nif->cond, 0, fp);
        fprintf(fp, " {\n");
        ast_node_dump(nif->stats, indent + 1, fp);
        fprintf(fp, "%s}", buf);
        struct ast_node* next = nif->opt_else;
        while (next) {
            struct ast_node_if* elif = next->node;
            fprintf(fp, " else ");
            if (elif->cond) {
                fprintf(fp, "if ");
                ast_node_dump(elif->cond, 0, fp);
            }
            fprintf(fp, " {\n");
            ast_node_dump(elif->stats, indent + 1, fp);
            fprintf(fp, "%s}", buf);
            next = elif->opt_else;
        }
    } break;

    case AST_NODE_OP: {
        struct ast_node_op* op = node->node;
        fprintf(fp, "%s", buf);
        ast_node_dump(op->left, 0, fp);
        switch (op->op) {
        case AST_NODE_OP_AND:
            fprintf(fp, " && ");
            break;
        case AST_NODE_OP_OR:
            fprintf(fp, " || ");
            break;
        case AST_NODE_OP_NOT:
            fprintf(fp, "!");
            break;
        case AST_NODE_OP_NEQ:
            fprintf(fp, " != ");
            break;
        case AST_NODE_OP_EQ:
            fprintf(fp, " == ");
            break;
        case AST_NODE_OP_GE:
            fprintf(fp, " >= ");
            break;
        case AST_NODE_OP_LE:
            fprintf(fp, " <= ");
            break;
        case AST_NODE_OP_GT:
            fprintf(fp, " > ");
            break;
        case AST_NODE_OP_LS:
            fprintf(fp, " < ");
            break;
        case AST_NODE_OP_ADD:
            fprintf(fp, " + ");
            break;
        case AST_NODE_OP_SUB:
            fprintf(fp, " - ");
            break;
        case AST_NODE_OP_MUL:
            fprintf(fp, " * ");
            break;
        case AST_NODE_OP_DIV:
            fprintf(fp, " / ");
            break;
        case AST_NODE_OP_MOD:
            fprintf(fp, " %% ");
            break;
        case AST_NODE_OP_DDOT:
            fprintf(fp, "..");
            break;
        case AST_NODE_OP_NEG:
            fprintf(fp, "-");
            break;
        default:
            unimplemented_("ast node op:%x", op->op);
        }
        ast_node_dump(op->right, 0, fp);
    } break;

    case AST_NODE_VARS:
    case AST_NODE_ARGS: {
        fprintf(fp, "%s", buf);
        struct ast_node_array* arr = node->node;
        for (size_t i = 0; i < arr->len; i++) {
            ast_node_dump(arr->arr[i], 0, fp);
            if (i != arr->len - 1)
                fprintf(fp, ", ");
        }
    } break;

    case AST_NODE_FOR: {
        struct ast_node_for* nfor = node->node;
        fprintf(fp, "%sfor ", buf);
        ast_node_dump(nfor->vars, 0, fp);
        fprintf(fp, " in ");
        ast_node_dump(nfor->iterator, 0, fp);
        fprintf(fp, " {\n");
        ast_node_dump(nfor->stats, indent + 1, fp);
        fprintf(fp, "}");
    } break;

    case AST_NODE_CALL: {
        struct ast_node_call* call = node->node;
        ast_node_dump(call->func, indent, fp);
        fprintf(fp, "(");
        ast_node_dump(call->args, 0, fp);
        fprintf(fp, ")");
    } break;

    case AST_NODE_BREAK:
        fprintf(fp, "%sbreak", buf);
        break;

    case AST_NODE_CONTINUE:
        fprintf(fp, "%scontinue", buf);
        break;

    case AST_NODE_RETURN: {
        fprintf(fp, "%sreturn ", buf);
        ast_node_dump(node->node, 0, fp);
    } break;

    case AST_NODE_VALUE:
        fprintf(fp, "%s", buf);
        hg_value_write(*(struct hg_value*)node->node, fp, true);
        break;

    case AST_NODE_INDEX: {
        struct ast_node_index* idx = node->node;
        fprintf(fp, "%s", buf);
        ast_node_dump(idx->xs, 0, fp);
        fprintf(fp, "[");
        ast_node_dump(idx->idx, 0, fp);
        fprintf(fp, "]");
    } break;

    default:
        unimplemented_("type: %x", node->type);
    }
}
//< ast_node_dump
#undef assert_expr
