#include <assert.h>
#include <stdio.h>

#include "ast_node.h"
#include "hedgehog.h"

extern int yyparse(struct parser_state* p);

void ast_node_dup(struct ast_node* node, int indent) {
    if (node == NULL)
        return;
    char buf[1024];

    int len = 0;
    for (; len < indent * 2; len++) {
        buf[len] = ' ';
    }
    buf[len] = '\0';

    switch (node->type) {
    case AST_NODE_STATS: {
        struct ast_node_array* arr = node->node;
        for (int i = 0; i < arr->len; i++) {
            ast_node_dup(arr->arr[i], indent);
            printf("\n");
        }
    } break;
    case AST_NODE_FUNC: {
        struct ast_node_func* func = node->node;
        printf("%sdef ", buf);
        ast_node_dup(func->id, 0);
        printf("(");
        ast_node_dup(func->vars, 0);
        printf(") {\n");
        ast_node_dup(func->stats, indent + 1);
        printf("%s}", buf);
    } break;
    case AST_NODE_WHILE: {
        struct ast_node_while* whl = node->node;
        printf("%swhile ", buf);
        ast_node_dup(whl->cond, 0);
        printf(" {\n");
        ast_node_dup(whl->stats, indent + 1);
        printf("%s}", buf);
    } break;
    case AST_NODE_VALUE: {
        struct hg_value* val = node->node;
        switch (val->type) {
        case HG_VALUE_ID: {
            printf("%s%s", buf, VAL_AS_ID(*val));
        } break;
        case HG_VALUE_STRING: {
            printf("%s\"%s\"", buf, VAL_AS_STR(*val));
        } break;
        case HG_VALUE_INT:
            printf("%s%ld", buf, VAL_AS_INT(*val));
            break;
        case HG_VALUE_FLOAT:
            printf("%s%lf", buf, VAL_AS_FLOAT(*val));
            break;
        case HG_VALUE_LIST: {
            printf("%s[", buf);
            ast_node_dup(VAL_AS_PTR(*val), 0);
            printf("]");
        } break;
        case HG_VALUE_NIL:
            printf("%snil", buf);
            break;
        case HG_VALUE_BOOL:
            printf("%s%s", buf, VAL_AS_BOOL(*val) ? "true" : "false");
            break;
        case HG_VALUE_TUPLE: {
            printf("%s(", buf);
            ast_node_dup(VAL_AS_PTR(*val), 0);
            printf(")");
        } break;
        default:
            printf("Error: not implemented");
        }
    } break;
    case AST_NODE_ASSIGN: {
        struct ast_node_assign* asg = node->node;
        printf("%s", buf);
        ast_node_dup(asg->vars, 0);
        printf(" = ");
        ast_node_dup(asg->args, 0);
    } break;
    case AST_NODE_IF: {
        struct ast_node_if* nif = node->node;
        printf("%sif ", buf);
        ast_node_dup(nif->cond, 0);
        printf(" {\n");
        ast_node_dup(nif->stats, indent + 1);
        printf("%s}", buf);
        struct ast_node* next = nif->opt_else;
        while (next) {
            struct ast_node_if* elif = next->node;
            printf(" else ");
            if (elif->cond) {
                printf("if ");
                ast_node_dup(elif->cond, 0);
            }
            printf(" {\n");
            ast_node_dup(elif->stats, indent + 1);
            printf("%s}", buf);
            next = elif->opt_else;
        }
    } break;
    case AST_NODE_OP: {
        struct ast_node_op* op = node->node;
        printf("%s", buf);
        ast_node_dup(op->left, 0);
        switch (op->op) {
        case AST_NODE_OP_AND:
            printf(" && ");
            break;
        case AST_NODE_OP_OR:
            printf(" || ");
            break;
        case AST_NODE_OP_NOT:
            printf(" ! ");
            break;
        case AST_NODE_OP_NEQ:
            printf(" != ");
            break;
        case AST_NODE_OP_EQ:
            printf(" == ");
            break;
        case AST_NODE_OP_GE:
            printf(" >= ");
            break;
        case AST_NODE_OP_LE:
            printf(" <= ");
            break;
        case AST_NODE_OP_GT:
            printf(" > ");
            break;
        case AST_NODE_OP_LS:
            printf(" < ");
            break;
        case AST_NODE_OP_ADD:
            printf(" + ");
            break;
        case AST_NODE_OP_SUB:
            printf(" - ");
            break;
        case AST_NODE_OP_MUL:
            printf(" * ");
            break;
        case AST_NODE_OP_DIV:
            printf(" / ");
            break;
        case AST_NODE_OP_MOD:
            printf(" %% ");
            break;
        case AST_NODE_OP_DDOT:
            printf(" .. ");
            break;
        default:
            printf(" ERROR ");
        }
        ast_node_dup(op->right, 0);
    } break;

    case AST_NODE_VARS:
    case AST_NODE_ARGS: {
        printf("%s", buf);
        struct ast_node_array* arr = node->node;
        for (int i = 0; i < arr->len; i++) {
            ast_node_dup(arr->arr[i], 0);
            if (i != arr->len - 1)
                printf(", ");
        }
    } break;
    case AST_NODE_FOR: {
        struct ast_node_for* nfor = node->node;
        printf("%sfor ", buf);
        ast_node_dup(nfor->vars, 0);
        printf(" in ");
        ast_node_dup(nfor->iterator, 0);
        printf(" {\n");
        ast_node_dup(nfor->stats, indent + 1);
        printf("}");
    } break;
    case AST_NODE_CALL: {
        struct ast_node_call* call = node->node;
        ast_node_dup(call->id, indent);
        ast_node_dup(call->tuple, 0);
    } break;
    case AST_NODE_BREAK:
        printf("%sbreak", buf);
        break;
    case AST_NODE_CONTINUE:
        printf("%scontinue", buf);
        break;
    case AST_NODE_RETURN: {
        printf("%sreturn ", buf);
        struct ast_node_return* ret = node->node;
        ast_node_dup(ret->expr, 0);
    } break;
    }
}

int main(int argc, char** argv) {
    extern FILE* yyin;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        return -1;
    }

    FILE* f;
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "faild to open `%s`\n", argv[1]);
        return -1;
    }

    yyin = f;

    struct parser_state p = {
        .lineno = 1, .tline = 1, .lval = NULL, .fname = argv[1]};

    while (yyparse(&p))
        ;
    ast_node_dup(p.lval, 0);
    return 0;
}
