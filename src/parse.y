%{
#include "common.h"
#include "ast.h"

#include "yy.hedgehog.h"

#define YYDEBUG 1
#define YYERROR_VERBOSE 1


#ifdef YYAC_DEBUG
#define print printf
#else
#define print(...)
#endif

%}

%define api.pure          full
%define api.prefix        {hg_yy}
// %define api.symbol.prefix {hg_yy_s_}
%define api.token.prefix  {hg_yy_t_}
%define parse.error       verbose
%define parse.lac         full

%parse-param {struct hg_parser* p} {void* scanner}
%lex-param {p}{scanner}

%union {
    hg_char            s;
    hg_int             i;
    hg_float           d;
    struct hg_ast_node *node;
}

%{
int yylex(YYSTYPE* lval, struct hg_parser* p, void* scanner);
static void yyerror(struct hg_parser* p, void* scanner, const char* s);
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

%token lit_true lit_false lit_string lit_id lit_int lit_float 

%%

program:
    optional_seps stats optional_seps {
        p->ast = $<node>2;
    }
    ;

stats:
    stat {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_STATS);
        $<node>$ = hg_ast_node_array_append(p, $<node>$, $<node>1);
    }
    |
    stats seps stat {
        $<node>$ = hg_ast_node_array_append(p, $<node>1, $<node>3);
        print("\n%d: shift stat to stats (append)\n", __LINE__);
    }
    |
    stats error {
        $<node>$ = $<node>1;
    }
    |
    error {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_STATS);
    }
    ;

block:
    sep_lp sep_rp {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_STATS);
        print("\n%d: empty block \n", __LINE__);
    }
    |
    sep_lp stats sep_rp {
        $<node>$ = $<node>2;
        print("\n%d: stats block\n", __LINE__);
    }
    ;

optional_seps:
    /* none */ {
    }
    |
    seps
    ;

seps:
    sep
    |
    seps sep {
        yyerrok;
    }
    ;

sep:
    sep_semic
    |
    sep_nl {
       yyerrok;
    }
    ;

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
        $<node>$ = hg_ast_expr_stat_new(p, $<node>1);
        print("\n%d: reduce expr to stat\n", __LINE__);
    }
    ;

function_stat:
    kw_fn ident sep_lb sep_rb block {
        $<node>$ = hg_ast_func_stat_new(p, $<node>2, NULL, $<node>5);
        print("\n%d: function without parameters\n", __LINE__);
    }
    |
    kw_fn ident sep_lb parameters sep_rb block {
        $<node>$ = hg_ast_func_stat_new(p, $<node>2, $<node>4, $<node>6);
        print("\n%d: function without\n", __LINE__);
    }
    ;

parameters:
    ident {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_PARAMS);
        $<node>$ = hg_ast_node_array_append(p, $<node>$, $<node>1);
    }
    |
    parameters sep_comma ident {
        $<node>$ = hg_ast_node_array_append(p, $<node>1, $<node>3);
    }
    ;

assignment_stat:
    vars op_assign exprs {
        $<node>$ = hg_ast_assignment_stat_new(p, $<node>1, $<node>3);
    }
    ;

vars:
    var_declarator {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_VARS);
        $<node>$ = hg_ast_node_array_append(p, $<node>$, $<node>1);
    }
    |
    vars sep_comma var_declarator {
        $<node>$ = hg_ast_node_array_append(p, $<node>1, $<node>3);
    }
    ;

var_declarator:
    ident
    |
    index_expr
    |
    field_expr
    ;

for_stat:
    kw_for parameters kw_in expr block {
        $<node>$ = hg_ast_for_stat_new(p, $<node>2, $<node>4, $<node>5);
    }
    ;

continue_stat:
    kw_continue {
        $<node>$ = hg_ast_continue_stat_new(p);
    }
    ;

break_stat:
    kw_break {
        $<node>$ = hg_ast_break_stat_new(p);
    }
    ;

return_stat:
    kw_return {
        $<node>$ = hg_ast_return_stat_new(p, NULL);
        print("\n%d: return nothing\n", __LINE__);
    }
    |
    kw_return exprs {
        print("\n%d: return expr\n", __LINE__);
        $<node>$ = hg_ast_return_stat_new(p, $<node>2);
    };

if_stat:
    kw_if expr block {
        $<node>$ = hg_ast_if_stat_new(p, $<node>2, $<node>3, NULL);
        print("\n%d: if stat\n", __LINE__);
    }
    |
    kw_if expr block else_stat {
        $<node>$ = hg_ast_if_stat_new(p, $<node>2, $<node>3, $<node>4);
        print("\n%d: if-else stat\n", __LINE__);
    };

else_stat:
    kw_else block {
        $<node>$ = $<node>2;
        print("\n%d: else expr\n", __LINE__);
    };

exprs:
    expr {
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_EXPRS);
        $<node>$ = hg_ast_node_array_append(p, $<node>$, $<node>1);
    }
    |
    exprs sep_comma expr {
        $<node>$ = hg_ast_node_array_append(p, $<node>1, $<node>3);
    }
    ;

function_def_expr:
    kw_fn sep_lb sep_rb block {
        $<node>$ = hg_ast_func_def_new(p, NULL, $<node>4);
    }
    |
    kw_fn sep_lb parameters sep_rb block {
        $<node>$ = hg_ast_func_def_new(p, $<node>3, $<node>5);
    }
    ;

function_call_expr:
    primary sep_lb sep_rb {
        $<node>$ = hg_ast_call_expr_new(p, $<node>1, NULL);
        print("\n%d: call function\n", __LINE__);
    }
    |
    primary sep_lb exprs sep_rb {
        $<node>$ = hg_ast_call_expr_new(p, $<node>1, $<node>3);
    }
    ;

index_expr:
    primary sep_ls expr sep_rs {
        $<node>$ = hg_ast_index_expr_new(p, $<node>1, $<node>3);
    }
    ;

field_expr:
    primary op_dot ident {
        $<node>$ = hg_ast_field_expr_new(p, $<node>1, $<node>3);
    }
    ;

table_expr:
    sep_lp sep_rp {
        $<node>$ = hg_ast_table_expr_new(p, NULL);
        print("\n%d: empty table\n", __LINE__);
    }
    |
    sep_lp exprs sep_rp {
        $<node>$ = hg_ast_table_expr_new(p, $<node>2);
    }
    |
    sep_lp table_entris sep_rp {
        $<node>$ = hg_ast_table_expr_new(p, $<node>2);
    }
    ;

table_entris:
    expr sep_colon expr {
        print("\n%d: reduce expr:expr to entries\n", __LINE__);
        $<node>$ = hg_ast_node_array_new(p, HG_AST_NODE_ARRAY_TABLE_ENTRIES);
        $<node>$ = hg_ast_node_array_append(p, $<node>$, hg_ast_table_entry_new(p, $<node>1, $<node>3));
    }
    |
    table_entris sep_comma expr sep_colon expr {
        print("\n%d: append table entries\n", __LINE__);
        $<node>$ = hg_ast_node_array_append(p, $<node>1, hg_ast_table_entry_new(p, $<node>3, $<node>5));
    }
    ;


expr:
    primary {
    }
    |
    op_sub primary {
        $<node>$ = hg_ast_unary_expr_new(p, HG_AST_NODE_OP_SUB, $<node>2);
    }
    |
    op_not expr {
        $<node>$ = hg_ast_unary_expr_new(p, HG_AST_NODE_OP_NOT, $<node>2);
    }
    |
    expr op_and expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_AND, $<node>1, $<node>2);
    }
    |
    expr op_or expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_OR, $<node>1, $<node>2);
    }
    |
    expr op_ls expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_OR, $<node>1, $<node>2);
    }
    |
    expr op_gt expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_GT, $<node>1, $<node>2);
    }
    |
    expr op_ge expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_GE, $<node>1, $<node>2);
    }
    |
    expr op_le expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_LE, $<node>1, $<node>2);
    }
    |
    expr op_eq expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_EQ, $<node>1, $<node>2);
    }
    |
    expr op_neq expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_NEQ, $<node>1, $<node>2);
    }
    |
    expr op_add expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_ADD, $<node>1, $<node>2);
    }
    |
    expr op_sub expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_SUB, $<node>1, $<node>2);
    }
    |
    expr op_mod expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_MOD, $<node>1, $<node>2);
    }
    |
    expr op_div expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_DIV, $<node>1, $<node>2);
    }
    |
    expr op_mul expr {
        $<node>$ = hg_ast_binary_expr_new(p, HG_AST_NODE_OP_MUL, $<node>1, $<node>2);
    }
    ;

ident:
    lit_id {
        $<node>$ = hg_ast_literal_id_new(p, $<s>1);
    }
    ;

primary:
    lit_string {
        print("`%s`:string\n", $<s>1);
        $<node>$ = hg_ast_literal_str_new(p, $<s>1);
    }
    |
    lit_true {
        $<node>$ = hg_ast_literal_bool_new(p, true);
    }
    |
    lit_false {
        $<node>$ = hg_ast_literal_bool_new(p, false);
    }
    |
    lit_int{
        print("%ld:int\n", $<i>1);
        $<node>$ = hg_ast_literal_int_new(p, $<i>1);
    }
    |
    lit_float {
        print("%lf:double\n", $<d>1);
        $<node>$ = hg_ast_literal_float_new(p, $<d>1);
    }
    |
    ident
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

static void yyerror(struct hg_parser* p, void* scanner,const char* s) {
    p->nerr++;
    if (p->fname) {
        fprintf(stderr, "%s:%d:%s:\n`%s`\n", p->fname, p->lineno, s, hg_yyget_text(scanner));
    } else {
        fprintf(stderr, "%d:%s:\n`%s`\n", p->lineno, s, hg_yyget_text(scanner));
    }
}
