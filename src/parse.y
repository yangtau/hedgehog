%{
#include "ast_node.h"

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
%right op_not
%left op_dot
%left op_or
%left op_and 
%left op_neq op_eq op_ge op_le op_gt op_ls
%left op_add op_sub
%left op_mul op_div op_mod

%token sep_lp sep_rp // {}
       sep_ls sep_rs // ()
       sep_lb sep_rb // ()
       sep_colon // :
       sep_nl sep_semic sep_comma // \n;,
       kw_if kw_else kw_for kw_break kw_continue kw_return
       kw_in kw_fn

%token <node> lit_float lit_int lit_bool lit_string lit_symbol
%type <node>  primary expr func_call args params stat if_stat opt_elsif_stat
              for_stat func_def stats block program comp_stat
              index_expr map_expr list_expr entries dot_expr func_expr
              assignable assignable_list
              callable

%%

program:
    comp_stat {
        // p->lval = $1;
    };

comp_stat:
    opt_sep stats opt_sep {
//         $$ = $2;
    };

block:
    sep_lp sep_rp {
//         $$ = NULL; 
    }
    |
    sep_lp comp_stat sep_rp {
//         $$ = $2; 
    };

stats:
    stat {
//         $$ = ast_node_array_new(p, AST_NODE_STATS);
//        ast_node_array_add(p, $$, $1);
    }
    |
    stats seps stat {
//         $$ = $1;
//        ast_node_array_add(p, $$, $3);
    }
    |
    stats error {
    }
    |
    error {
//         $$ = NULL;
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
    assignable_list op_assign args { /* a, b = 1, 2 */
//         $$ = ast_node_assign_new(p, $1, $3);
    }
    |
    kw_break {
//         $$ = ast_node_loopctrl_new(p, AST_NODE_BREAK);
    }
    |
    kw_continue {
//         $$ = ast_node_loopctrl_new(p, AST_NODE_CONTINUE);
    }
    |
    kw_return {
//         $$ = ast_node_return_new(p, NULL);
    }
    |
    kw_return expr {
//         $$ = ast_node_return_new(p, $2);
    };


assignable_list:
    assignable
    |
    assignable_list sep_comma assignable;

assignable:
    lit_symbol
    |
    index_expr
    |
    dot_expr
    ;

if_stat:
    kw_if expr block opt_elsif_stat {
//         $$ = ast_node_if_new(p, $2, $3, $4);
    };

opt_elsif_stat:
    /* none */ {
//        $$ = NULL;
    }
    |
    kw_else block {
//         $$ = ast_node_if_new(p, NULL, $2, NULL);
    }
    |
    kw_else kw_if expr block opt_elsif_stat {
//         $$ = ast_node_if_new(p, $3, $4, $5);
    };

for_stat:
    kw_for params kw_in expr block {
//         $$ = ast_node_for_new(p, $2, $4, $5);
    };

func_def:
    kw_fn lit_symbol sep_lb sep_rb block {
//         $$ = ast_node_func_new(p, $2, NULL, $5);
    }
    |
    kw_fn lit_symbol sep_lb params sep_rb block {
//         $$ = ast_node_func_new(p, $2, $4, $6);
    };

func_expr:
    kw_fn sep_lb sep_rb block {
    }
    |
    kw_fn sep_lb params sep_rb block {
        // $$ = ast_node_func_new(p, $2, $4, $6);
    };

params:
    lit_symbol {
        // $$ = ast_node_array_new(p, AST_NODE_VARS);
        // ast_node_array_add(p, $$, $1);
    }
    |
    params sep_comma lit_symbol {
        // ast_node_array_add(p, $$, $3);
        // $$ = $1;
    };

list_expr:
    sep_lp args sep_comma sep_rp {
        // $$ = ast_node_list_new(p, $2);
    }
    |
    sep_lp args sep_rp {
//         $$ = ast_node_list_new(p, $2);
    };

map_expr:
    sep_lp entries sep_comma sep_rp {
//         $$ = ast_node_map_new(p, $2);
    }
    |
    sep_lp entries sep_rp {
//         $$ = ast_node_map_new(p, $2);
    };

entries:
    expr sep_colon expr {
//         $$ = ast_node_array_new(p, AST_NODE_ARGS);
//        ast_node_array_add(p, $$, $1);
//        ast_node_array_add(p, $$, $3);
    }
    |
    entries sep_comma expr sep_colon expr {
//        ast_node_array_add(p, $$, $3);
//        ast_node_array_add(p, $$, $5);
//         $$ = $1;
    };

args:
    /* none */ {
//         $$ = NULL;
    }
    |
    expr {
//         $$ = ast_node_array_new(p, AST_NODE_ARGS);
//        ast_node_array_add(p, $$, $1);
    }
    |
    args sep_comma expr {
//        ast_node_array_add(p, $$, $3);
//         $$ = $1;
    };

expr:
    primary
    |
    op_sub primary {
//         $$ = ast_node_op_new(p, AST_NODE_OP_NEG, NULL, $2);
    }
    | // TODO: not should have higher precedence than `and` and `or`
    op_not expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_NOT, NULL, $2);
    }
    |
    expr op_and expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_AND, $1, $3);
    }
    |
    expr op_or expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_OR, $1, $3);
    } 
    |
    expr op_ls expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_LS, $1, $3);
    } 
    |
    expr op_gt expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_GT, $1, $3);
    }
    |
    expr op_ge expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_GE, $1, $3);
    }
    |
    expr op_le expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_LE, $1, $3);
    }
    |
    expr op_eq expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_EQ, $1, $3);
    }
    |
    expr op_neq expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_NEQ, $1, $3);
    }
    |
    expr op_add expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_ADD, $1, $3);
    }
    |
    expr op_sub expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_SUB, $1, $3);
    }
    |
    expr op_mod expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_MOD, $1, $3);
    }
    |
    expr op_div expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_DIV, $1, $3);
    }
    |
    expr op_mul expr {
//         $$ = ast_node_op_new(p, AST_NODE_OP_MUL, $1, $3);
    };

func_call:
    callable sep_lb args sep_rb { /* f(a, b) */
//         $$ = ast_node_call_new(p, $1, $3);
    };

callable:
    lit_symbol
    |
    dot_expr
    |
    func_expr
    |
    func_call
    ;

index_expr:
    primary sep_ls expr sep_rs {
//         $$ = ast_node_index_new(p, $1, $3);
    };

dot_expr:
    primary op_dot lit_symbol {
//         $$ = ast_node_dot_new(p, $1, $3);
    };

primary:
    lit_string
    |
    lit_bool
    |
    lit_int
    |
    lit_float
    |
    lit_symbol 
    |
    func_call
    |
    index_expr
    |
    map_expr
    |
    dot_expr
    |
    func_expr
    |
    list_expr;

%%

#include "lex.hedgehog.c"

static void yyerror(struct parser_state* p, const char* s) {
    p->nerr++;
    if (p->fname) {
        fprintf(stderr, "%s:%d:%s:\n`%s`\n", p->fname, p->lineno, s, yytext);
    } else {
        fprintf(stderr, "%d:%s:\n`%s`\n", p->lineno, s, yytext);
    }
}
