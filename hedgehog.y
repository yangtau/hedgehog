%{
// #define YYDEBUG 1
#include <stdint.h>
#include <stdio.h>
#include "hedgehog.h"

%}
%union {
    char *identifier;
    Value value;
    Statement *statement;
    Expression *expression;
}


%right ASSIGN
%token AND
%token OR

%token EQ GE LE NQ GT LT

%left MUL DIV MOD
%left ADD SUB
%token POWER


%right NOT

%token LP RP
%token  CR TAB

%token <identifier> IDENTIFIER
%token <value> DOUBLE INT BOOL NULL_V 
%type <value> VALUE FUNCTION_CALL_EXPRESSION
%type <expression> VALUE_EXPRESSION  UNARY_EXPRESSION POWER_EXPRESSION
                MUL_EXPRESSION ADD_EXPRESSION GREATER_EXPRESSION
                EQUAL_EXPRESSION AND_EXPRESSION OR_EXPRESSION
                EXPRESSION
%type <statement> STATEMENT LINE

%%
STATEMENT:
    LINE 
    |
    STATEMENT LINE
    ;

LINE:
    EXPRESSION CR{
        Value v = evaluateExpression($1);
        freeExpression($1);
        log("%s","expressin free");
        valuePrint(v);
        printf("\n>>");
        $$ = createStatement();
    };

EXPRESSION:
    OR_EXPRESSION
    |
    IDENTIFIER ASSIGN EXPRESSION {
        $$ = createAssignExpression($1, $3);
    }
    ;

OR_EXPRESSION:
    AND_EXPRESSION
    |
    OR_EXPRESSION OR AND_EXPRESSION {
        $$ = createBinaryExpression(OR_OPERATOR, $1, $3);
    }
    ;

AND_EXPRESSION:
    EQUAL_EXPRESSION
    |
    AND_EXPRESSION AND EQUAL_EXPRESSION {
        $$ = createBinaryExpression(AND_OPERATOR, $1, $3);
    }
    ;

EQUAL_EXPRESSION:
    GREATER_EXPRESSION
    |
    EQUAL_EXPRESSION EQ GREATER_EXPRESSION {
        $$ = createBinaryExpression(EQ_OPERATOR, $1, $3);
    }
    |
    EQUAL_EXPRESSION NQ GREATER_EXPRESSION {
        $$ = createBinaryExpression(NQ_OPERATOR, $1, $3);
    }
    ;

GREATER_EXPRESSION:
    ADD_EXPRESSION
    |
    GREATER_EXPRESSION LT ADD_EXPRESSION {
        $$ = createBinaryExpression(LT_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION LE ADD_EXPRESSION {
        $$ = createBinaryExpression(LE_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION GT ADD_EXPRESSION {
        $$ = createBinaryExpression(GT_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION GE ADD_EXPRESSION {
        $$ = createBinaryExpression(GE_OPERATOR, $1, $3);
    }
    ;

ADD_EXPRESSION:
    MUL_EXPRESSION
    |
    ADD_EXPRESSION ADD MUL_EXPRESSION {
        $$ = createBinaryExpression(ADD_OPERATOR, $1, $3);
    }
    |
    ADD_EXPRESSION SUB MUL_EXPRESSION {
        $$ = createBinaryExpression(SUB_OPERATOR, $1, $3);
    }
    ;

MUL_EXPRESSION:
    UNARY_EXPRESSION
    |
    MUL_EXPRESSION MUL UNARY_EXPRESSION {
        $$ = createBinaryExpression(MUL_OPERATOR, $1, $3);
    }
    |
    MUL_EXPRESSION DIV UNARY_EXPRESSION {
        $$ = createBinaryExpression(DIV_OPERATOR, $1, $3);
    }
    |
    MUL_EXPRESSION MOD UNARY_EXPRESSION {
        $$ = createBinaryExpression(MOD_OPERATOR, $1, $3);
    }
    ;

UNARY_EXPRESSION:
    POWER_EXPRESSION
    |
    SUB UNARY_EXPRESSION {
        $$ = createUnaryExpression(SUB_OPERATOR, $2);
    }
    |
    NOT UNARY_EXPRESSION {
        $$ = createUnaryExpression(NOT_OPERATOR, $2);
    }
    ;

POWER_EXPRESSION:
    VALUE_EXPRESSION
    |
    POWER_EXPRESSION POWER VALUE_EXPRESSION {
        $$ = createBinaryExpression(POWER_OPERATOR, $1, $3);
    }
    ;

VALUE_EXPRESSION:
    VALUE {
       $$ = createValueExpression($1);
    }
    |
    LP EXPRESSION RP {
        $$ = $2;
    }
    ;

VALUE:
    INT
    |
    DOUBLE
    |
    BOOL
    |
    NULL_V
    |
    IDENTIFIER {
        $$ = findVariable(getCurrentInterpreter()->globalEnv, $1);
    }
    |
    FUNCTION_CALL_EXPRESSION
    ;

FUNCTION_CALL_EXPRESSION:
    IDENTIFIER LP RP {

    }
    ;

%%