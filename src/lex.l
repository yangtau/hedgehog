%{
#include "yy.hedgehog.h"

#include "common.h"
#include "string.h"

#define YY_DECL int hg_yy_lex(YYSTYPE* lval, struct hg_parser_state* p)

static int lex_return(struct hg_parser_state* p, int c) {
    p->lineno = p->tline;
    p->tline = yylineno;
#ifdef LEX_DEBUG
    switch (c) {
        case hg_yy_t_op_dot: printf(".");                          break;
        case hg_yy_t_op_assign: printf("=");                       break;
        case hg_yy_t_op_or: printf("||");                          break;
        case hg_yy_t_op_and: printf("&&");                         break;
        case hg_yy_t_op_not: printf("!");                          break;
        case hg_yy_t_op_neq: printf("!=");                         break;
        case hg_yy_t_op_eq: printf("==");                          break;
        case hg_yy_t_op_ge: printf(">=");                          break;
        case hg_yy_t_op_le: printf("<=");                          break;
        case hg_yy_t_op_gt: printf(">");                           break;
        case hg_yy_t_op_ls: printf("<");                           break;
        case hg_yy_t_op_add: printf("+");                          break;
        case hg_yy_t_op_sub: printf("-");                          break;
        case hg_yy_t_op_mul: printf("*");                          break;
        case hg_yy_t_op_div: printf("/");                          break;
        case hg_yy_t_op_mod: printf("%%");                         break;
        case hg_yy_t_sep_lp: printf("{");                          break;
        case hg_yy_t_sep_rp: printf("}");                          break;
        case hg_yy_t_sep_ls: printf("[");                          break;
        case hg_yy_t_sep_rs: printf("]");                          break;
        case hg_yy_t_sep_lb: printf("(");                          break;
        case hg_yy_t_sep_rb: printf(")");                          break;
        case hg_yy_t_sep_nl: printf("\\n\n");                      break;
        case hg_yy_t_sep_semic: printf(";");                       break;
        case hg_yy_t_sep_comma: printf(",");                       break;
        case hg_yy_t_sep_colon: printf(":");                       break;
        case hg_yy_t_kw_if: printf("if ");                         break;
        case hg_yy_t_kw_else: printf("else ");                     break;
        case hg_yy_t_kw_for: printf("for ");                       break;
        case hg_yy_t_kw_break: printf("brk ");                     break;
        case hg_yy_t_kw_continue: printf("conti ");                break;
        case hg_yy_t_kw_return: printf("ret ");                    break;
        case hg_yy_t_kw_fn: printf("fn ");                         break;
        case hg_yy_t_kw_in: printf("in ");                         break;
        case hg_yy_t_lit_int: printf("%s:int ", hg_yytext);       break;
        case hg_yy_t_lit_string: printf("`%s`:str ", hg_yytext);  break;
        case hg_yy_t_lit_float: printf("%s:float ", hg_yytext);   break;
        case hg_yy_t_lit_false: printf("false:bool ", hg_yytext); break;
        case hg_yy_t_lit_true: printf("true:bool ", hg_yytext);   break;
        case hg_yy_t_lit_id: printf("@%s", hg_yytext);            break;
        default: printf("#ERROR");
    }
    fflush(stdout);
#endif
    return c;
}

#define LEX_RETURN(c) return lex_return(p, c)
%}

%option prefix="hg_yy"
%option yylineno
%option noyywrap

TRAIL  ([\t \n]|"#"[^\n]*"\n")*
%%
"."{TRAIL}                      LEX_RETURN(hg_yy_t_op_dot);
"+"{TRAIL}                      LEX_RETURN(hg_yy_t_op_add);
"-"{TRAIL}                      LEX_RETURN(hg_yy_t_op_sub);
"*"{TRAIL}                      LEX_RETURN(hg_yy_t_op_mul);
"/"{TRAIL}                      LEX_RETURN(hg_yy_t_op_div);
"%"{TRAIL}                      LEX_RETURN(hg_yy_t_op_mod);
"=="{TRAIL}                     LEX_RETURN(hg_yy_t_op_eq);
"!="{TRAIL}                     LEX_RETURN(hg_yy_t_op_neq);
"<"{TRAIL}                      LEX_RETURN(hg_yy_t_op_ls);
"<="{TRAIL}                     LEX_RETURN(hg_yy_t_op_le);
">"{TRAIL}                      LEX_RETURN(hg_yy_t_op_gt);
">="{TRAIL}                     LEX_RETURN(hg_yy_t_op_ge);
"&&"{TRAIL}                     LEX_RETURN(hg_yy_t_op_and);
"||"{TRAIL}                     LEX_RETURN(hg_yy_t_op_or);
"!"{TRAIL}                      LEX_RETURN(hg_yy_t_op_not);
"="{TRAIL}                      LEX_RETURN(hg_yy_t_op_assign);
":"{TRAIL}                      LEX_RETURN(hg_yy_t_sep_colon);
"["{TRAIL}                      LEX_RETURN(hg_yy_t_sep_ls);
"("{TRAIL}                      LEX_RETURN(hg_yy_t_sep_lb);
"{"{TRAIL}                      LEX_RETURN(hg_yy_t_sep_lp);
","{TRAIL}                      LEX_RETURN(hg_yy_t_sep_comma);
{TRAIL}"]"                      LEX_RETURN(hg_yy_t_sep_rs);
{TRAIL}")"                      LEX_RETURN(hg_yy_t_sep_rb);
{TRAIL}"}"                      LEX_RETURN(hg_yy_t_sep_rp);
";"{TRAIL}                      LEX_RETURN(hg_yy_t_sep_semic);
"\n"{TRAIL}                     LEX_RETURN(hg_yy_t_sep_nl);
"#"[^\n]*"\n"                   LEX_RETURN(hg_yy_t_sep_nl);
{TRAIL}"in"{TRAIL}              LEX_RETURN(hg_yy_t_kw_in);
{TRAIL}"else"{TRAIL}            LEX_RETURN(hg_yy_t_kw_else);
"if"{TRAIL}                     LEX_RETURN(hg_yy_t_kw_if);
"break"{TRAIL}                  LEX_RETURN(hg_yy_t_kw_break);
"continue"{TRAIL}               LEX_RETURN(hg_yy_t_kw_continue);
"return"{TRAIL}                 LEX_RETURN(hg_yy_t_kw_return);
"for"{TRAIL}                    LEX_RETURN(hg_yy_t_kw_for);
"fn"{TRAIL}                     LEX_RETURN(hg_yy_t_kw_fn);

"true" {
    LEX_RETURN(hg_yy_t_lit_true);
};

"false" {
    LEX_RETURN(hg_yy_t_lit_false);
};

[A-Za-z_][A-Za-z0-9_]* {
    lval->s = hg_string_new(yytext, strlen(yytext));
    LEX_RETURN(hg_yy_t_lit_id);
};

(([1-9][0-9]*)|0) {
    hg_int t;
    sscanf(yytext, "%" HGFormatInt, &t);
    lval->i = t;
    LEX_RETURN(hg_yy_t_lit_int);
};

(([1-9][0-9]*)|0)(\.[0-9]+) {
    hg_float t;
    sscanf(yytext, "%" HGFormatFloat, &t);
    lval->d = t;
    LEX_RETURN(hg_yy_t_lit_float);
};

\"([^\\\"]|\\.)*\" {
    lval->s = hg_string_new(yytext+1, strlen(yytext)-2);
    LEX_RETURN(hg_yy_t_lit_string);
};

[ \t] ;

. {
    if (p->fname) {
        fprintf(stderr, "%s:%d:lexical error:\n `%s`\n", p->fname, p->lineno, hg_yytext);
    } else {
        fprintf(stderr, "%d:lexical error:\n `%s`\n", p->lineno, hg_yytext);
    }
};
%%
