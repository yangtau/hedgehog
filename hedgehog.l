%{
#include <stdio.h>
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

<INITIAL>"==" return EQ:
<INITIAL>"!=" return NQ;
<INITIAL>">" return GT;
<INITIAL>"<" return LT;
<INITIAL>">=" return GE;
<INITIAL>"<=" return LE;

<INITIAL>"(" return LP;
<INITIAL>")" return RP;

<INITIAL>\t return TAB; 

<INITIAL>"true" {
    return TRUE_VALUE;
}
<INITIAL>"false" {
    return FALSE_VALUE;
}
<INITIAL>"null" {
    return NULL_VALUE;
}


<INITIAL>[0-9]+ {
    sscanf(yytext, "%" PRId32, );
}
<INITIAL>[0-9]+\.[0-9]+ {
    sscanf(yytext, "%lf" , );
}

<INITIAL>[A-Za-z_][A-Za-z0-9_]* {
    
}
<INITIAL>[ ];
%%