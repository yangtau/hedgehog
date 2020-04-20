%{
#include <string.h>
#include "ast_node.h"
#include "yy.hedgehog.h"

#define YY_DECL int yylex(YYSTYPE *lval, struct parser_state *p)

static int lex_return(struct parser_state *p, int c) {
    p->lineno = p->tline;
    p->tline = yylineno;
    return c;
}

#define LEX_RETURN(c) return lex_return(p, c)
%}

%option yylineno
%option noyywrap

TRAIL  ([\t \n]|"#"[^\n]*"\n")*
%%
"+"{TRAIL}  LEX_RETURN(op_add);
"-"{TRAIL}  LEX_RETURN(op_sub);
"*"{TRAIL}  LEX_RETURN(op_mul);
"/"{TRAIL}  LEX_RETURN(op_div);
"%"{TRAIL}  LEX_RETURN(op_mod);
"=="{TRAIL} LEX_RETURN(op_eq);
"!="{TRAIL} LEX_RETURN(op_neq);
"<"{TRAIL}  LEX_RETURN(op_ls);
"<="{TRAIL} LEX_RETURN(op_le);
">"{TRAIL}  LEX_RETURN(op_gt);
">="{TRAIL} LEX_RETURN(op_ge);
"&&"{TRAIL} LEX_RETURN(op_and);
"||"{TRAIL} LEX_RETURN(op_or);
"!"{TRAIL}  LEX_RETURN(op_not);
"="{TRAIL}  LEX_RETURN(op_assign);
"{"{TRAIL}  LEX_RETURN(sep_lp);
"("{TRAIL}  LEX_RETURN(sep_lb);
"}"         LEX_RETURN(sep_rp);
")"         LEX_RETURN(sep_rb);
","{TRAIL}  LEX_RETURN(sep_comma);
";"{TRAIL}  LEX_RETURN(sep_semic);
"\n"          LEX_RETURN(sep_nl);
"#"[^\n]*"\n" LEX_RETURN(sep_nl);

"if"{TRAIL}       LEX_RETURN(kw_if);
"else"{TRAIL}     LEX_RETURN(kw_else);
"break"{TRAIL}    LEX_RETURN(kw_break);
"continue"{TRAIL} LEX_RETURN(kw_continue);
"return"{TRAIL}   LEX_RETURN(kw_return);
"for"{TRAIL}      LEX_RETURN(kw_for);
"func"{TRAIL}     LEX_RETURN(kw_func);

"nil"{TRAIL} {
    lval->node = ast_node_nil_new();
    LEX_RETURN(lit_null);
}
"true"{TRAIL} {
    lval->node = ast_node_bool_new(1);
    LEX_RETURN(lit_bool);
}
"false"{TRAIL} {
    lval->node = ast_node_bool_new(0);
    LEX_RETURN(lit_bool);
}

[A-Za-z_][A-Za-z0-9_]* {
    lval->node = ast_node_id_new(yytext);
    LEX_RETURN(lit_id);
};

(([1-9][0-9]*)|0) {
    int t;
    sscanf(yytext, "%d", &t);
    lval->node = ast_node_int_new(t);
    LEX_RETURN(lit_int);
}

(([1-9][0-9]*)|0)(\.[0-9]*) {
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
    fprintf(stderr, "lexical error ('%s').\n", yytext); 
    exit(1); 
};
%% 
