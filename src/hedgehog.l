%{
/* Created by Tau on 05/02/2019 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "hedgehog.h"
#include "y.tab.h"
int yywrap(void) { return 1; }

char *string_buf;
int cnt = 0;
int size = 256;
void start_string_buf() {
    string_buf = calloc(sizeof(char), size);
    cnt = 0;
}

void add_char_to_string_buf(char c) {
    string_buf[cnt++] = c;
    if (cnt+1>=size) {
        size*=2;
        string_buf = realloc(string_buf, sizeof(char)*size);
    }
    string_buf[cnt] = '\0';
}

void close_string_buf() {
    free(string_buf);
    string_buf = NULL;
    cnt = 0;
    size=1024;
}

%}
%start COMMENT_STATE STRING_STATE

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
<INITIAL>"," return COMMA;
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

<INITIAL>\" {
    start_string_buf();
    BEGIN STRING_STATE;
}
<INITIAL>"//" {
    BEGIN COMMENT_STATE;
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

<STRING_STATE>\" {
    yylval.value.type = STRING_VALUE;
    yylval.value.v.string_value = initString(string_buf);
    close_string_buf();
    BEGIN INITIAL;
    return STRING;
}
<STRING_STATE>\\\" add_char_to_string_buf('\"');
<STRING_STATE>\\\\ add_char_to_string_buf('\\');
<STRING_STATE>\\n add_char_to_string_buf('\n');
<STRING_STATE>\\t add_char_to_string_buf('\t');
<STRING_STATE>. add_char_to_string_buf(yytext[0]);

<COMMENT_STATE>\n BEGIN INITIAL;
<COMMENT_STATE>. ;
%%
