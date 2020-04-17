%{
#include "hedgehog.h"
%}
%union {
    String* id;
    Value value;
    void *statement;
    void *expression;
    StatementList *statement_list;
    ArgumentList *argument_list;
    ParameterList *parameter_list;
}

%right op_assign
%left kw_and kw_or
%right kw_not
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
%type <parameter_list> paras
%type <statement> stat if_else_stat if_stat for_stat func_def
%type <statement_list> stats block program

%%

program:
    stats opt_sep {
        StatementList* list = getCurrentInterpreter()->list;
        $$ = list->add(list, $1);
    };

block:
    sep_lp stats sep_rp {
        $$ = $2;
    };

stats:
    stat {
        $$ = initStatementList();
        $$->add($$, $1);
    }
    |
    stats seps stat {
        $$ = $1->add($1, $3);
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
        $$ = initExpressionStatement($1);
    }
    |
    paras op_assign args { /* a, b = 1, 2 */
    	$$ = initMultiAssignExpression($1, $3);
    }
    |
    kw_break {
    	$$ = initBreakStatement();
    }
    |
    kw_continue {
        $$ = initContinueStatement();
    }
    |
    kw_return {
    	$$ = initReturnStatement(NULL);
    }
    |
    kw_return expression {
    	$$ = initReturnStatement($2);
    }
    ;

if_else_stat:
    if_stat
    |
    if_stat kw_else block {
        $1->addElse($1, $3);
        $$ = $1;
    };

if_stat:
    kw_if expr block {
        $$ = initIfStatement($2, $3);
    }
    |
    if_stat kw_else kw_if expr block {
        $1->addElsIf($1, $4, $5);
        $$ = $1;
    };

for_stat:
    kw_for expr sep_semic expr sep_semic expr block {
        $$ = initForStatement($2, $4, $6, $7);
    }
    |
    kw_for expr block {
        $$ = initForStatement(NULL, $2, NULL, $3);
    }
    |
    kw_for block {
        $$ = initForStatement(NULL, NULL, NULL, $2);
    }
    ;

func_def:
    kw_func lit_id sep_lb sep_rb block {
        $$ = initFunctionDefineStatement($2, NULL, $5);
    }
    |
    kw_func lit_id sep_lb paras sep_rb block {
        $$ = initFunctionDefineStatement($2, $4, $6);
    };

paras:
    lit_id {
        $$ = initParameterList($1);
    }
    |
    paras sep_comma lit_id {
        $$ = $1->add($1, $3);
    };

func_call_expr:
    lit_id sep_lb sep_rb { /* fn() */
        $$ = initFunctionCallExpression($1, NULL);
    }
    |
    lit_id sep_lb args sep_rb { /* fn(a, b) */
        $$ = initFunctionCallExpression($1, $3);
    };

args:
    expr {
        $$ = initParameterList($1);
    }
    |
    args sep_comma expr {
        $$ = $1->add($1, $3);
    };

expr:
    and_expr;

and_expr:
    cmp_expr
    |
    kw_not cmp_expr {
        $$ = initUnaryExpression(NOT_OPERATOR, $2); 
    }
    |
    and_expr kw_and cmp_expr {
        $$ = initBinaryExpression(AND_OPERATOR, $1, $3);
    }
    |
    and_expr kw_or cmp_expr {
        $$ = initBinaryExpression(OR_OPERATOR, $1, $3);
    } 
    ;

cmp_expr:
    add_expr
    |
    cmp_expr op_ls add_expr {
        $$ = initBinaryExpression(LT_OPERATOR, $1, $3);
    }
    |
    cmp_expr op_gt add_expr {
        $$ = initBinaryExpression(GT_OPERATOR, $1, $3);
    }
    |
    cmp_expr op_ge add_expr {
        $$ = initBinaryExpression(GE_OPERATOR, $1, $3);
    }
    |
    cmp_expr op_le add_expr {
        $$ = initBinaryExpression(LE_OPERATOR, $1, $3);
    }
    |
    cmp_expr op_eq add_expr {
        $$ = initBinaryExpression(EQ_OPERATOR, $1, $3);
    }
    |
    cmp_expr op_neq add_expr {
        $$ = initBinaryExpression(NQ_OPERATOR, $1, $3);
    }
    ;
add_expr:
    value_expr
    |
    add_expr op_add value_expr {
        $$ = initBinaryExpression(ADD_OPERATOR, $1, $3);
    }
    |
    add_expr op_sub value_expr {
        $$ = initBinaryExpression(SUB_OPERATOR, $1, $3);
    }
    |
    add_expr op_mod value_expr {
        $$ = initBinaryExpression(MOD_OPERATOR, $1, $3);
    }
    |
    add_expr op_div value_expr {
        $$ = initBinaryExpression(DIV_OPERATOR, $1, $3);
    }
    |
    add_expr op_mul value_expr {
        $$ = initBinaryExpression(MUL_OPERATOR, $1, $3);
    }
    ;


value_expr:
    func_call_expr
    |
    factor {
        $$ = initValueExpression($1);
    }
    |
    sep_lb expr sep_rb {
        $$ = $2;
    }
    |
    lit_id {
        $$ = initVariableExpression($1);
    }
    |
    op_sub value_expr { /* -2 */
        $$ = initUnaryExpression(SUB_OPERATOR, $2);
    }
    ;

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
