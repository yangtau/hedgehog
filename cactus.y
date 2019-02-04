%{
// #define YYDEBUG 1
#include <stdint.h>
#include <stdio.h>
#include "cactus.h"
%}
%union {
    char *identifier;
    Value expression_value;
}

%token <identifier> IDENTIFIER
%token <expression_value> DOUBLE 
%token <expression_value> INT
%token <expression_value> BOOL
%token <expression_value> STRING
%token ASSIGN
%token CR
%type <expression_value> expression value

%%
line_list:
    line |
    line_list line
    ;
line:
    expression CR {
        #ifdef CMD_MOD
        printValue($1);
        printf("\n");
        SHOW_HEADER;
        #endif
    }
    ;

expression:
    value |
    IDENTIFIER ASSIGN expression {
        addToGlobalVariableList(createVariable($1, $3));
        $$ = $3;
    }
    ;
    
value:
    DOUBLE |
    INT |
    BOOL |
    STRING |
    IDENTIFIER {
        Variable *var = searchLocalVariable($1);
        log("serch var type:%d", var->v.type);
        Value v;
        if (var == NULL) {
            v.type = NULL_VALUE;
        } else {
            v = var->v;
        }
        $$ = v;
    }
    ;
%%

int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "--parser error:\n%s\n", yytext);
    return 0;
}

int main() {
    extern FILE* yyin;
    globalVariableList.next = NULL;
    yyin = stdin;
    SHOW_HEADER;
    yyparse();
    return 0;
}