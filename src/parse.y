%{
#include "ast_node.h"
#include "hedgehog.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

%}

%define api.pure
%parse-param {struct parser_state* p}
%lex-param {p}

%union {
    struct ast_node* node;
}
 
%{
int yylex(YYSTYPE* lval, struct parser_state* p);
static void yyerror(struct parser_state* p, const char* s);
%}

// precedence table
%right op_assign
%left op_or
%left op_and 
%right op_not
%left op_neq op_eq op_ge op_le op_gt op_ls
%left op_add op_sub
%left op_mul op_div op_mod

%token sep_lp // {
       sep_rp // }
       sep_lb // (
       sep_rb // )
       sep_nl // \n
       sep_semic // ;
       sep_comma // ,
       kw_if kw_else kw_for kw_break kw_continue kw_return
       kw_func

%token <node> lit_float lit_int lit_bool lit_null lit_string lit_id
%type <node>  primary expr func_call_expr args vars stat if_stat opt_elsif_stat
              for_stat func_def stats block program comp_stat

%%

program:
    comp_stat {
        p->lval = $1;
    };

comp_stat:
    stats opt_sep {
    };

block:
    sep_lp sep_rp {
        $$ = NULL; 
    }
    |
    sep_lp comp_stat sep_rp {
        $$ = $2; 
    };

stats:
    stat {
        $$ = ast_node_array_new(AST_NODE_STATS, $1);
    }
    |
    stats seps stat {
        $$ = $1;
        ast_node_array_add($$, $3);
    };

opt_sep:
    /* none */
    |
    seps;

seps:
    sep
    | 
    seps sep {
        yyerrok;
    };

sep:
    sep_semic {
       yyerrok;
    }
    |
    sep_nl;

stat:
    func_def
    |
    for_stat
    |
    if_stat
    |
    expr
    |
    vars op_assign args { /* a, b = 1, 2 */
        $$ = ast_node_assign_new($1, $3);
    }
    |
    kw_break {
        $$ = ast_node_loopctrl_new(AST_NODE_BREAK);
    }
    |
    kw_continue {
        $$ = ast_node_loopctrl_new(AST_NODE_CONTINUE);
    }
    |
    kw_return {
        $$ = ast_node_return_new(NULL);
    }
    |
    kw_return expr {
        $$ = ast_node_return_new($2);
    };

if_stat:
    kw_if expr block opt_elsif_stat {
        $$ = ast_node_if_new($2, $3, $4);
    };

opt_elsif_stat:
    /* none */ {
       $$ = NULL;
    }
    |
    kw_else block {
        $$ = ast_node_if_new(NULL, $2, NULL);
    }
    |
    kw_else kw_if expr block opt_elsif_stat {
        $$ = ast_node_if_new($3, $4, $5);
    };

for_stat:
    kw_for expr sep_semic expr sep_semic expr block {
        $$ = ast_node_for_new($2, $4, $6, $7);
    }
    |
    kw_for expr block {
        $$ = ast_node_for_new(NULL, $2, NULL, $3);
    }
    |
    kw_for block {
        $$ = ast_node_for_new(NULL, NULL, NULL, $2);
    };

func_def:
    kw_func lit_id sep_lb sep_rb block {
        $$ = ast_node_func_new($2, NULL, $5);
    }
    |
    kw_func lit_id sep_lb vars sep_rb block {
        $$ = ast_node_func_new($2, $4, $6);
    };

vars:
    lit_id {
        $$ = ast_node_array_new(AST_NODE_VARS, $1);
    }
    |
    vars sep_comma lit_id {
        ast_node_array_add($$, $3);
        $$ = $1;
    };

func_call_expr:
    lit_id sep_lb sep_rb { /* fn() */
        $$ = ast_node_call_new($1, NULL);
    }
    |
    lit_id sep_lb args sep_rb { /* fn(a, b) */
        $$ = ast_node_call_new($1, $3);
    };

args:
    expr {
        $$ = ast_node_array_new(AST_NODE_ARGS, $1);
    }
    |
    args sep_comma expr {
        ast_node_array_add($$, $3);
        $$ = $1;
    };

expr:
    primary
    |
    op_sub primary {
        $$ = ast_node_op_new(AST_NODE_OP_SUB, NULL, $2);
    }
    |
    op_not expr {
        $$ = ast_node_op_new(AST_NODE_OP_NOT, NULL, $2);
    }
    |
    expr op_and expr {
        $$ = ast_node_op_new(AST_NODE_OP_AND, $1, $3);
    }
    |
    expr op_or expr {
        $$ = ast_node_op_new(AST_NODE_OP_OR, $1, $3);
    } 
    |
    expr op_ls expr {
        $$ = ast_node_op_new(AST_NODE_OP_LS, $1, $3);
    } 
    |
    expr op_gt expr {
        $$ = ast_node_op_new(AST_NODE_OP_GT, $1, $3);
    }
    |
    expr op_ge expr {
        $$ = ast_node_op_new(AST_NODE_OP_GE, $1, $3);
    }
    |
    expr op_le expr {
        $$ = ast_node_op_new(AST_NODE_OP_LE, $1, $3);
    }
    |
    expr op_eq expr {
        $$ = ast_node_op_new(AST_NODE_OP_EQ, $1, $3);
    }
    |
    expr op_neq expr {
        $$ = ast_node_op_new(AST_NODE_OP_NEQ, $1, $3);
    }
    |
    expr op_add expr {
        $$ = ast_node_op_new(AST_NODE_OP_ADD, $1, $3);
    }
    |
    expr op_sub expr {
        $$ = ast_node_op_new(AST_NODE_OP_SUB, $1, $3);
    }
    |
    expr op_mod expr {
        $$ = ast_node_op_new(AST_NODE_OP_MOD, $1, $3);
    }
    |
    expr op_div expr {
        $$ = ast_node_op_new(AST_NODE_OP_DIV, $1, $3);
    }
    |
    expr op_mul expr {
        $$ = ast_node_op_new(AST_NODE_OP_MUL, $1, $3);
    };

primary:
    lit_string
    |
    lit_null
    |
    lit_bool
    |
    lit_int
    |
    lit_float
    |
    lit_id 
    |
    func_call_expr
    |
    sep_lb expr sep_rb {
        $$ = $2;
    };

%%

#include "lex.hedgehog.c"

static void yyerror(struct parser_state* p, const char* s) {
    p->nerr++;
    if (p->fname) {
        fprintf(stderr, "%s:%d:%s\n", p->fname, p->lineno, s);
    } else {
        fprintf(stderr, "%s\n", s);
    }
}
