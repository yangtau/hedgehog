#include "ast_node.h"
#include "value.h"
#include "memory.h"

#define assert_expr(node)                                        \
    {                                                            \
        enum ast_node_type type = (node)->type;                  \
        assert(type == AST_NODE_VALUE || type == AST_NODE_OP ||  \
               type == AST_NODE_CALL || type == AST_NODE_LIST || \
               type == AST_NODE_TUPLE);                          \
    }

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
struct ast_node* ast_node_op_new(enum ast_node_op_type type,
                                 struct ast_node* left,
                                 struct ast_node* right) {
    if (left == NULL)
        assert(type == AST_NODE_OP_NEG || type == AST_NODE_OP_NOT);
    else
        assert_expr(left);
    assert_expr(right);

    struct ast_node* node       = ast_node_new(AST_NODE_OP);
    struct ast_node_op* node_op = hg_alloc_(struct ast_node_op);

    node_op->op    = type;
    node_op->left  = left;
    node_op->right = right;

    node->node = node_op;
    return node;
}
//< ast_node_op

//> ast_node_if
struct ast_node* ast_node_if_new(struct ast_node* cond, struct ast_node* stats,
                                 struct ast_node* opt_else) {
    if (opt_else != NULL)
        assert(opt_else->type == AST_NODE_IF);
    if (cond != NULL)
        assert_expr(cond);

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
struct ast_node* ast_node_call_new(struct ast_node* id,
                                   struct ast_node* tuple) {
    assert(id->type == AST_NODE_VALUE || id->type == AST_NODE_CALL);
    assert(tuple->type == AST_NODE_TUPLE);

    struct ast_node* node           = ast_node_new(AST_NODE_CALL);
    struct ast_node_call* node_call = hg_alloc_(struct ast_node_call);

    node_call->id    = id;
    node_call->tuple = tuple;

    node->node = node_call;
    return node;
}
//< ast_node_call

//> ast_node_func
struct ast_node* ast_node_func_new(struct ast_node* id, struct ast_node* vars,
                                   struct ast_node* stats) {
    assert(id->type == AST_NODE_VALUE);
    if (vars != NULL)
        assert(vars->type == AST_NODE_VARS);
    if (stats != NULL)
        assert(stats->type == AST_NODE_STATS);

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
struct ast_node* ast_node_array_new(enum ast_node_type type) {
    struct ast_node* node = ast_node_new(type);
    struct ast_node_array* arr =
        flexible_alloc_(struct ast_node_array, struct ast_node*, 1);

    arr->capacity = 1;
    arr->len      = 0;

    node->node = arr;
    return node;
}

void ast_node_array_add(struct ast_node* arr, struct ast_node* item) {
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

struct ast_node* ast_node_list_new(struct ast_node* args) {
    if (args != NULL)
        assert(args->type == AST_NODE_ARGS);

    if (args == NULL)
        args = ast_node_array_new(AST_NODE_LIST);

    args->type = AST_NODE_LIST;
    return args;
}

struct ast_node* ast_node_tuple_new(struct ast_node* args) {
    if (args != NULL)
        assert(args->type == AST_NODE_ARGS);

    if (args == NULL)
        args = ast_node_array_new(AST_NODE_TUPLE);

    args->type = AST_NODE_TUPLE;
    return args;
}
//< ast_node_array

//> ast_node_assign
struct ast_node* ast_node_assign_new(struct ast_node* vars,
                                     struct ast_node* args) {
    assert(vars->type == AST_NODE_VARS);
    assert(args->type == AST_NODE_ARGS);

    struct ast_node* node               = ast_node_new(AST_NODE_ASSIGN);
    struct ast_node_assign* node_assign = hg_alloc_(struct ast_node_assign);

    node_assign->vars = vars;
    node_assign->args = args;

    node->node = node_assign;
    return node;
}
//< ast_node_assign

//> ast_node_return
struct ast_node* ast_node_return_new(struct ast_node* expr) {
    if (expr != NULL)
        assert_expr(expr);

    struct ast_node* node = ast_node_new(AST_NODE_RETURN);

    node->node = expr;
    return node;
}
//< ast_node_return

//> ast_node_while
struct ast_node* ast_node_while_new(struct ast_node* cond,
                                    struct ast_node* stats) {
    assert_expr(cond);
    if (stats != NULL)
        assert(stats->type == AST_NODE_STATS);

    struct ast_node* node             = ast_node_new(AST_NODE_WHILE);
    struct ast_node_while* node_while = hg_alloc_(struct ast_node_while);

    node_while->cond  = cond;
    node_while->stats = stats;

    node->node = node_while;
    return node;
}
//< ast_node_while

//> ast_node_for
struct ast_node* ast_node_for_new(struct ast_node* vars,
                                  struct ast_node* iterator,
                                  struct ast_node* stats) {
    assert(vars->type == AST_NODE_VARS);
    assert_expr(iterator);
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
struct ast_node* ast_node_loopctrl_new(enum ast_node_type type) {
    assert(type == AST_NODE_CONTINUE || type == AST_NODE_BREAK);

    return ast_node_new(type);
}
//< ast_node_loopctrl

//> ast_node_value
struct ast_node* ast_node_int_new(int64_t v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_INT(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_float_new(double v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_FLOAT(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_bool_new(bool v) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_BOOL(v);

    node->node = value;
    return node;
}

struct ast_node* ast_node_str_new(const char* s) {
    return ast_node_str_len_new(s, strlen(s));
}

struct ast_node* ast_node_str_len_new(const char* s, size_t len) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_STR_LEN(s, len);

    node->node = value;
    return node;
}

struct ast_node* ast_node_id_new(const char* s) {
    struct ast_node* node  = ast_node_new(AST_NODE_VALUE);
    struct hg_value* value = hg_alloc_(struct hg_value);

    *value = VAL_ID(s);

    node->node = value;
    return node;
}
//< ast_node_value

//> ast_node_free
void ast_node_free(struct ast_node* node) {
    if (node == NULL)
        return;
    switch (node->type) {
    case AST_NODE_ASSIGN: {
        struct ast_node_assign* node_assign = node->node;
        ast_node_free(node_assign->args);
        ast_node_free(node_assign->vars);
        hg_free_(struct ast_node_assign, node_assign);
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
        ast_node_free(node_call->id);
        ast_node_free(node_call->tuple);
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
        ast_node_dump(call->id, indent, fp);
        ast_node_dump(call->tuple, 0, fp);
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

    case AST_NODE_VALUE: {
        fprintf(fp, "%s", buf);
        struct hg_value* val = node->node;
        switch (val->type) {
        case HG_VALUE_INT:
        case HG_VALUE_BOOL:
        case HG_VALUE_NIL:
        case HG_VALUE_FLOAT:
            hg_value_write(*val, fp);
            break;
        case HG_VALUE_OBJECT: {
            struct hg_object* obj = VAL_AS_OBJ(*val);
            if (obj->type == HG_OBJ_STRING)
                fprintf(fp, "\"");
            hg_value_write(*val, fp);
            if (obj->type == HG_OBJ_STRING)
                fprintf(fp, "\"");

        } break;
        default:
            unimplemented_("hg_value type: %x", val->type);
        }
    } break;
    default:
        unimplemented_("type: %x", node->type);
    }
}
//< ast_node_dump
#undef assert_expr
