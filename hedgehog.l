%{
/* Created by Tau on 05/02/2019 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "hedgehog.h"
#include "y.tab.h"
int yywrap(void) { return 1; }
%}

%%
<INITIAL>"+" return ADD;
<INITIAL>"-" return SUB;
<INITIAL>"*" return MUL;
<INITIAL>"/" return DIV;
<INITIAL>"%" return MOD;
<INITIAL>"**" return POWER;
<INITIAL>"=" return ASSIGN;

<INITIAL>"|" return OR;
<INITIAL>"&" return AND;
<INITIAL>"!" return NOT;

<INITIAL>"==" return EQ;
<INITIAL>"!=" return NQ;
<INITIAL>">" return GT;
<INITIAL>"<" return LT;
<INITIAL>">=" return GE;
<INITIAL>"<=" return LE;

<INITIAL>"(" return LP;
<INITIAL>")" return RP;

<INITIAL>"{" return LB;
<INITIAL>"}" return RB;

<INITIAL>"if" return IF;
<INITIAL>"else" return ELSE;
<INITIAL>"elsif" return ELSE_IF;
<INITIAL>"for" return FOR;
<INITIAL>"break" return BREAK;
<INITIAL>"continue" return CONTINUE;
<INITIAL>"return" return RETURN;
<INITIAL>"as" return AS;
<INITIAL>"with" return WITH;
<INITIAL>"in" return IN;
<INITIAL>"on" return ON;
<INITIAL>"switch" return SWITCH;
<INITIAL>";" return SEMIC;
<INITIAL>"func" return FUNC;
<INITIAL>\n ;

<INITIAL>"true" {
    yylval.value.type = BOOL_VALUE;
    yylval.value.v.bool_value = 1;
    return BOOL;
}
<INITIAL>"false" {
    yylval.value.type = BOOL_VALUE;
    yylval.value.v.bool_value = 0;
    return BOOL;
}
<INITIAL>"null" {
    yylval.value.type = NULL_VALUE;
    return NULL_V;
}


<INITIAL>[0-9]+ {
    yylval.value.type = INT_VALUE;
    sscanf(yytext, "%" PRId32, &yylval.value.v.int_value);
    return INT;
}
<INITIAL>[0-9]+\.[0-9]+ {
    yylval.value.type = DOUBLE_VALUE;
    sscanf(yytext, "%lf" , &yylval.value.v.double_value);
    return DOUBLE;
} 

<INITIAL>[A-Za-z_][A-Za-z0-9_]* {
    yylval.identifier = initString(yytext);
    return IDENTIFIER;
}
<INITIAL>[ \t];
%%
// <INITIAL>"\n{\n" return LB;
// <INITIAL>"{\n" return LB;
// <INITIAL>"\n{" return LB;
// <INITIAL>"\n}" return RB;