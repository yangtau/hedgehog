%{
#include <string.h>
#include "ast_node.h"
#include "yy.hedgehog.h"

#define YY_DECL int yylex(YYSTYPE* lval, struct parser_state* p)

static int lex_return(struct parser_state* p, int c) {
    p->lineno = p->tline;
    p->tline = yylineno;
// #define LEX_DEBUG
#ifdef LEX_DEBUG
    switch (c) {
        case op_ddot: printf(".."); break;
        case op_assign: printf("="); break;
        case op_or: printf("||"); break;
        case op_and: printf("&&"); break;
        case op_not: printf("!"); break;
        case op_neq: printf("!="); break;
        case op_eq: printf("=="); break;
        case op_ge: printf(">="); break;
        case op_le: printf("<="); break;
        case op_gt: printf(">"); break;
        case op_ls: printf("<"); break;
        case op_add: printf("+"); break;
        case op_sub: printf("-"); break;
        case op_mul: printf("*"); break;
        case op_div: printf("/"); break;
        case op_mod: printf("%%"); break;
        case sep_lp: printf("{\n"); break;
        case sep_rp: printf("}"); break;
        case sep_ls: printf("["); break;
        case sep_rs: printf("]"); break;
        case sep_lb: printf("("); break;
        case sep_rb: printf(")"); break;
        case sep_nl: printf("\\n\n"); break;
        case sep_semic: printf(";"); break;
        case sep_comma: printf(","); break;
        case kw_if: printf("if "); break;
        case kw_else: printf("else "); break;
        case kw_for: printf("for "); break;
        case kw_break: printf("brk "); break;
        case kw_continue: printf("conti "); break;
        case kw_return: printf("ret "); break;
        case kw_def: printf("def "); break;
        case kw_in: printf("in "); break;
        case kw_while: printf("while "); break;
        case lit_int: printf("%s:int ", yytext);break;
        case lit_string: printf("%s:str ", yytext);break;
        case lit_float: printf("%s:float ", yytext);break;
        case lit_bool:
        case lit_id:
            printf("%s ", yytext); break;
        default: printf("#ERROR");
    }
    fflush(stdout);
#endif
    return c;
}

#define LEX_RETURN(c) return lex_return(p, c)
%}

%option yylineno
%option noyywrap

TRAIL  ([\t \n]|"#"[^\n]*"\n")*
%%
".."{TRAIL}                     LEX_RETURN(op_ddot);
"+"{TRAIL}                      LEX_RETURN(op_add);
"-"{TRAIL}                      LEX_RETURN(op_sub);
"*"{TRAIL}                      LEX_RETURN(op_mul);
"/"{TRAIL}                      LEX_RETURN(op_div);
"%"{TRAIL}                      LEX_RETURN(op_mod);
"=="{TRAIL}                     LEX_RETURN(op_eq);
"!="{TRAIL}                     LEX_RETURN(op_neq);
"<"{TRAIL}                      LEX_RETURN(op_ls);
"<="{TRAIL}                     LEX_RETURN(op_le);
">"{TRAIL}                      LEX_RETURN(op_gt);
">="{TRAIL}                     LEX_RETURN(op_ge);
"&&"{TRAIL}                     LEX_RETURN(op_and);
"||"{TRAIL}                     LEX_RETURN(op_or);
"!"{TRAIL}                      LEX_RETURN(op_not);
"="{TRAIL}                      LEX_RETURN(op_assign);

"["{TRAIL}                      LEX_RETURN(sep_ls);
"("{TRAIL}                      LEX_RETURN(sep_lb);
"{"{TRAIL}                      LEX_RETURN(sep_lp);
{TRAIL}"]"                      LEX_RETURN(sep_rs);
{TRAIL}")"                      LEX_RETURN(sep_rb);
{TRAIL}"}"                      LEX_RETURN(sep_rp);
{TRAIL}","{TRAIL}               LEX_RETURN(sep_comma);
{TRAIL}";"{TRAIL}               LEX_RETURN(sep_semic);
"\n"                            LEX_RETURN(sep_nl);
"#"[^\n]*"\n"                   LEX_RETURN(sep_nl);

{TRAIL}"in"{TRAIL}              LEX_RETURN(kw_in);
{TRAIL}"else"{TRAIL}            LEX_RETURN(kw_else);
"if"{TRAIL}                     LEX_RETURN(kw_if);
"break"{TRAIL}                  LEX_RETURN(kw_break);
"continue"{TRAIL}               LEX_RETURN(kw_continue);
"return"{TRAIL}                 LEX_RETURN(kw_return);
"for"{TRAIL}                    LEX_RETURN(kw_for);
"def"{TRAIL}                    LEX_RETURN(kw_def);
"while"{TRAIL}                  LEX_RETURN(kw_while);

"true" {
    lval->node = ast_node_bool_new(true);
    LEX_RETURN(lit_bool);
}
"false" {
    lval->node = ast_node_bool_new(false);
    LEX_RETURN(lit_bool);
}

[A-Za-z_][A-Za-z0-9_]* {
    lval->node = ast_node_id_new(yytext);
    LEX_RETURN(lit_id);
};

(([1-9][0-9]*)|0) {
    int64_t t;
    sscanf(yytext, "%ld", &t);
    lval->node = ast_node_int_new(t);
    LEX_RETURN(lit_int);
}

(([1-9][0-9]*)|0)(\.[0-9]+) {
    double t;
    sscanf(yytext, "%lf", &t);
    lval->node = ast_node_float_new(t);
    LEX_RETURN(lit_float);
};

\"([^\\\"]|\\.)*\" {
    lval->node = ast_node_str_len_new(yytext+1, strlen(yytext)-2);
    LEX_RETURN(lit_string);
};

[ \t] ; 

. { 
    fprintf(stderr, "%s:%d:lexical error:\n `%s`\n", p->fname, p->lineno, yytext); 
    exit(1);
};
%% 
