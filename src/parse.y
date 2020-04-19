%{
#include "node.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

%}

%define api.pure
%parse-param {parser_state *p}
%lex-param {p}

%union {
}
 
%{
int yylex(YYSTYPE *lval, parser_state *p);
static void yyerror(parser_state *p, const char *s);
%}



%right op_assign
%left op_and op_or
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

%token <id> lit_id
%token <value> lit_float lit_int lit_bool lit_null lit_string 

%type <value> factor
%type <expression> value_expr add_expr and_expr cmp_expr expr func_call_expr
%type <argument_list> args 
%type <parameter_list> vars
%type <statement> stat if_else_stat if_stat for_stat func_def
%type <statement_list> stats block program

%%

program:
    stats opt_sep {
    };

block:
    sep_lp stats sep_rp {
    };

stats:
    stat {
    }
    |
    stats seps stat {
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
    if_else_stat
    |
    expr {
    }
    |
    vars op_assign args { /* a, b = 1, 2 */
    }
    |
    kw_break {
    }
    |
    kw_continue {
    }
    |
    kw_return {
    }
    |
    kw_return expr {
    }
    ;

if_else_stat:
    if_stat
    |
    if_stat kw_else block {
    };

if_stat:
    kw_if expr block opt_elsif_stat {
    };

opt_elsif_stat:
    /* none */ {
       $$ = NULL;
    }
    |
    kw_else block {
    }
    |
    kw_else kw_if expr block opt_else_if_stat {
    };

for_stat:
    kw_for expr sep_semic expr sep_semic expr block {
    }
    |
    kw_for expr block {
    }
    |
    kw_for block {
    }
    ;

func_def:
    kw_func lit_id sep_lb sep_rb block {
    }
    |
    kw_func lit_id sep_lb vars sep_rb block {
    };

vars:
    lit_id {
    }
    |
    vars sep_comma lit_id {
    };

func_call_expr:
    lit_id sep_lb sep_rb { /* fn() */
    }
    |
    lit_id sep_lb args sep_rb { /* fn(a, b) */
    };

args:
    expr {
    }
    |
    args sep_comma expr {
    };

expr:
    and_expr;

and_expr:
    cmp_expr
    |
    op_not cmp_expr {
    }
    |
    and_expr op_and cmp_expr {
    }
    |
    and_expr op_or cmp_expr {
    } 
    ;

cmp_expr:
    add_expr
    |
    cmp_expr op_ls add_expr {
    }
    |
    cmp_expr op_gt add_expr {
    }
    |
    cmp_expr op_ge add_expr {
    }
    |
    cmp_expr op_le add_expr {
    }
    |
    cmp_expr op_eq add_expr {
    }
    |
    cmp_expr op_neq add_expr {
    };

add_expr:
    value_expr
    |
    add_expr op_add value_expr {
    }
    |
    add_expr op_sub value_expr {
    }
    |
    add_expr op_mod value_expr {
    }
    |
    add_expr op_div value_expr {
    }
    |
    add_expr op_mul value_expr {
    };


value_expr:
    func_call_expr
    |
    factor {
    }
    |
    sep_lb expr sep_rb {
    }
    |
    lit_id {
    }
    |
    op_sub value_expr { /* -2 */
    };

factor:
      lit_string
      |
      lit_null
      |
      lit_bool
      |
      lit_int
      |
      lit_float;
%%

#include "lex.hedgehog.c"

static void yyerror(parser_state *p, const char *s) {
    p->nerr++;
    if (p->fname) {
        fprintf(stderr, "%s:%d:%s\n", p->fname, p->lineno, s);
    } else {
        fprintf(stderr, "%s\n", s);
    }
}
