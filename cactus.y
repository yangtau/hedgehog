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
%token ASSIGN CR LP RP
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
    value 
    |
    IDENTIFIER {
        Variable *var = searchLocalVariable($1);
        Value v;
        if (var == NULL) {
            v.type = NULL_VALUE;
        } else {
            log("search var, type:%d", var->v.type);
            v = var->v;
        }
        $$ = v;
    }
    |
    IDENTIFIER ASSIGN expression {
        Variable *var = searchLocalVariable($1);
        if (var == NULL) {
            addToGlobalVariableList(createVariable($1, $3));
        }
        else {
            var->v = $3;
        }
        $$ = $3;
    }
    |
    IDENTIFIER LP RP {
        Value v =  callFunction( $1,NULL);
    }
    ;
    
value:
    DOUBLE |
    INT |
    BOOL |
    STRING |
    ;
%%

int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "ERROR:\nparser error:\n%s\n", yytext);
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