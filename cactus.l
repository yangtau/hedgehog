%{
#include <stdio.h>
#include <inttypes.h>
#include "cactus.h"
#include "y.tab.h"
int yywrap(void) { return 1; }
%}

%%
<INITIAL>"=" return ASSIGN;
<INITIAL>\n return CR;
<INITIAL>"(" return LP;
<INITIAL>")" return RP;
<INITIAL>"true" {
    yylval.expression_value.type = BOOL_VALUE;
    yylval.expression_value.u.bool_value.v = 1;
    return BOOL;
}
<INITIAL>"false" {
    yylval.expression_value.type = BOOL_VALUE;
    yylval.expression_value.u.bool_value.v = 0;
    return BOOL;
}

<INITIAL>[0-9]+ {
    // TODO: the len of num
    yylval.expression_value.type = INT_VALUE;
    sscanf(yytext, "%" PRId32, &yylval.expression_value.u.int_value);
    return INT;
}
<INITIAL>[0-9]+\.[0-9]+ {
    yylval.expression_value.type = DOUBLE_VALUE;
    sscanf(yytext, "%lf" , &yylval.expression_value.u.double_value);
    return DOUBLE;
}

<INITIAL>[A-Za-z_][A-Za-z0-9_]* {
    yylval.identifier = createIdentifier(yytext);
    return IDENTIFIER;
}


<INITIAL>[ \t];
%%