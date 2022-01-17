%{
#include "ast_node.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1


#ifdef YYAC_DEBUG
#define print printf
#else
#define print(...)
#endif

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

%token sep_lp sep_rp // {}
       sep_ls sep_rs // []
       sep_lb sep_rb // ()
       sep_colon // :
       sep_nl sep_semic sep_comma // \n;,
       kw_if kw_else kw_for kw_break kw_continue kw_return kw_in kw_fn
       op_dot

%token <node> lit_float lit_int lit_bool lit_string lit_symbol

%%

program:
    optional_seps stats optional_seps { }
    ;

stats:
    stat {
        print("\n%d: shift stat to stats\n", __LINE__);
    }
    |
    stats seps stat {
        print("\n%d: shift stat to stats (append)\n", __LINE__);
    }
    /*
    stats error {
        print("\n%d: shift stats with error to stats (append)\n", __LINE__);
    }
    |
    error {
        print("\n%d: shift error to stats (append)\n", __LINE__);
    }
    */
    ;

optional_seps:
    /* none */ {}
    |
    seps
    ;

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
    assignment_stat {
        print("\n%d: reduce assignment_stat to stat\n", __LINE__);
    }
    |
    if_stat {
        print("\n%d: reduce if_stat to stat\n", __LINE__);
    }
    |
    for_stat {
        print("\n%d: reduce for_stat to stat\n", __LINE__);
    }
    |
    function_stat {
        print("\n%d: reduce function_stat to stat\n", __LINE__);
    }
    |
    return_stat {
        print("\n%d: reduce return_stat to stat\n", __LINE__);
    }
    |
    continue_stat {
        print("\n%d: reduce continue_stat to stat\n", __LINE__);
    }
    |
    break_stat {
        print("\n%d: reduce break_stat to stat\n", __LINE__);
    }
    |
    expr {
        print("\n%d: reduce expr to stat\n", __LINE__);
    }
    ;

block:
    sep_lp sep_rp {
        print("\n%d: empty block \n", __LINE__);
    }
    |
    sep_lp stats sep_rp {
        print("\n%d: stats block\n", __LINE__);
    };


function_stat:
    kw_fn lit_symbol sep_lb sep_rb block {
        print("\n%d: function without parameters\n", __LINE__);
    }
    |
    kw_fn lit_symbol sep_lb parameters sep_rb block {
        print("\n%d: function without\n", __LINE__);
    }
    ;

parameters:
    lit_symbol
    |
    parameters sep_comma lit_symbol;

assignment_stat:
    vars op_assign exprs; 

vars:
    var_declarator
    |
    vars sep_comma var_declarator;

var_declarator:
    lit_symbol
    |
    index_expr
    |
    field_expr;

for_stat:
    kw_for parameters kw_in expr block;

continue_stat:
    kw_continue { };

break_stat:
    kw_break { };

return_stat:
    kw_return {
        print("\n%d: return nothing\n", __LINE__);
    }
    |
    kw_return expr {
        print("\n%d: return expr\n", __LINE__);
    };

if_stat:
    kw_if expr block {
        print("\n%d: if stat\n", __LINE__);
    }
    |
    kw_if expr block else_stat {
        print("\n%d: if-else stat\n", __LINE__);
    };

else_stat:
    kw_else block {
        print("\n%d: else expr\n", __LINE__);
    };

arguments:
    sep_lb sep_rb
    |
    sep_lb exprs sep_rb
    ;

exprs:
    expr
    |
    exprs sep_comma expr
    ;

function_def_expr:
    kw_fn sep_lb sep_rb block
    |
    kw_fn sep_lb parameters sep_rb block;

function_call_expr:
    primary arguments {
        print("\n%d: call function\n", __LINE__);
    }
    ;

index_expr:
    primary sep_ls expr sep_rs;

field_expr:
    primary op_dot lit_symbol;

table_expr:
    sep_lp sep_rp {
        print("\n%d: empty table\n", __LINE__);
    }
    |
    sep_lp exprs sep_rp
    |
    sep_lp table_entris sep_rp
    ;

table_entris:
    expr sep_colon expr {
        print("\n%d: reduce expr:expr to entries\n", __LINE__);
    }
    |
    table_entris sep_comma expr sep_colon expr {
        print("\n%d: append table entries\n", __LINE__);
    };


expr:
    primary {
    }
    |
    op_sub primary {
    }
    |
    op_not expr {
    }
    |
    expr op_and expr {
    }
    |
    expr op_or expr {
    } 
    |
    expr op_ls expr {
    } 
    |
    expr op_gt expr {
    }
    |
    expr op_ge expr {
    }
    |
    expr op_le expr {
    }
    |
    expr op_eq expr {
    }
    |
    expr op_neq expr {
    }
    |
    expr op_add expr {
    }
    |
    expr op_sub expr {
    }
    |
    expr op_mod expr {
    }
    |
    expr op_div expr {
    }
    |
    expr op_mul expr {
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
    table_expr {
        print("\n%d: reduce table to primary\n", __LINE__);
    }
    |
    function_call_expr
    |
    index_expr
    |
    field_expr
    |
    sep_lb expr sep_rb
    |
    function_def_expr;

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
