%{
#include <string.h>
#include "yy.tab.h"

#define YY_DECL int yylex(YYSTYPE *lval, parser_state *p)

static int lex_return(parser_state *p, int c) {
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
"\n"          LEX_RETURN('\n');
"#"[^\n]*"\n" LEX_RETURN('\n');

"if"{TRAIL}       LEX_RETURN(kw_if);
"else"{TRAIL}     LEX_RETURN(kw_else);
"break"{TRAIL}    LEX_RETURN(kw_break);
"continue"{TRAIL} LEX_RETURN(kw_continue);
"return"{TRAIL}   LEX_RETURN(kw_return);
"for"{TRAIL}      LEX_RETURN(kw_for);
"func"{TRAIL}     LEX_RETURN(kw_func);

"nil"{TRAIL} {
    LEX_RETURN(lit_null);
}
"true"{TRAIL} {
    LEX_RETURN(lit_bool);
}
"false"{TRAIL} {
    LEX_RETURN(lit_bool);
}

[A-Za-z_][A-Za-z0-9_]* {
    LEX_RETURN(lit_id);
};

(([1-9][0-9]*)|0) {
    LEX_RETURN(lit_int);
}

(([1-9][0-9]*)|0)(\.[0-9]*) {
    LEX_RETURN(lit_float);
};

\"([^\\\"]|\\.)*\"      {
    LEX_RETURN(lit_string);
};

[ \t] ; 

. { 
    fprintf(stderr, "lexical error ('%s').\n", yytext); 
    exit(1); 
};
%% 
