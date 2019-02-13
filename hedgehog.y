%{
/* Created by Tau on 05/02/2019 */

// #define YYDEBUG 1
#include <stdint.h>
#include <stdio.h>
#include "hedgehog.h"

%}
%union {
    String* identifier;
    Value value;
    void *statement;
    void *expression;
    StatementList *statement_list;
    ArgumentList *argument_list;
    ParameterList *parameter_list;
}


%right ASSIGN
%token AND
%token OR

%token EQ GE LE NQ GT LT

%left MUL DIV MOD
%left ADD SUB
%token POWER


%right NOT

%token LP RP LB RB

%token IF ELSE_IF ELSE FOR BREAK CONTINUE RETURN AS WITH IN ON
	SWITCH FUNC COMMA

%token  CR TAB SEMIC

%token <identifier> IDENTIFIER
%token <value> DOUBLE INT BOOL NULL_V STRING
%type <value> VALUE
%type <expression> VALUE_EXPRESSION  UNARY_EXPRESSION POWER_EXPRESSION
                MUL_EXPRESSION ADD_EXPRESSION GREATER_EXPRESSION
                EQUAL_EXPRESSION AND_EXPRESSION OR_EXPRESSION
                EXPRESSION FUNCTION_CALL_EXPRESSION
%type <argument_list> ARGUMENT_LIST
%type <parameter_list> PARAMETER_LIST
%type <statement> STATEMENT IF_STATEMENT FOR_STATEMENT FUNCTION_DEFINE_STATEMENT
%type <statement_list>  GLOBAL_LIST STATEMENT_LIST STATEMENT_BLOCK

%%

GLOBAL_LIST:
    STATEMENT {
    	StatementList* list = getCurrentInterpreter()->list;
    	$$ = list->add(list, $1);
    }
    |
    GLOBAL_LIST STATEMENT {
    	$$ = $1->add($1, $2);
    }
    ;

STATEMENT_BLOCK:
    LB STATEMENT_LIST RB {
        $$ = $2;
    }
    ;

STATEMENT_LIST:
    STATEMENT {
    	$$ =  initStatementList();
    	$$->add($$, $1);
    }
    |
    STATEMENT_LIST STATEMENT {
        $$ = $1->add($1, $2);
    }
    ;

STATEMENT:
    EXPRESSION SEMIC {
   	$$ = initExpressionStatement($1);
    }
    |
    IF_STATEMENT
    |
    FOR_STATEMENT
    |
    FUNCTION_DEFINE_STATEMENT
    |
    BREAK SEMIC{
    	$$ = initBreakStatement();
    }
    |
    CONTINUE SEMIC{
   	$$ = initContinueStatement();
    }
    |
    RETURN SEMIC {
    	$$ = initReturnStatement(NULL);
    }
    |
    RETURN OR_EXPRESSION SEMIC {
    	$$ = initReturnStatement($2);
    }
    ;

FOR_STATEMENT:
    FOR EXPRESSION SEMIC EXPRESSION SEMIC EXPRESSION STATEMENT_BLOCK {
        $$ = initForStatement($2, $4, $6, $7);
    }
    |
    FOR EXPRESSION STATEMENT_BLOCK {
        $$ = initForStatement(NULL, $2, NULL, $3);
    }
    |
    FOR STATEMENT_BLOCK {
    	$$ = initForStatement(NULL, NULL, NULL, $2);
    }
    ;

IF_STATEMENT:
    IF OR_EXPRESSION STATEMENT_BLOCK {
    	$$ = initIfStatement($2, $3);
    }
    |
    IF_STATEMENT ELSE_IF OR_EXPRESSION STATEMENT_BLOCK {
        IfStatement* ifS = $1;
    	ifS->addElsIf(ifS, $3, $4);
        $$=$1;
    }
    |
    IF_STATEMENT ELSE STATEMENT_BLOCK {
        IfStatement* ifS =  $1;
    	ifS->addElse(ifS, $3);
        $$=$1;
    }
    ;

FUNCTION_DEFINE_STATEMENT:
    FUNC IDENTIFIER LP RP STATEMENT_BLOCK {
        $$ = initFunctionDefineStatement($2, NULL, $5);
    }
    |
    FUNC IDENTIFIER LP PARAMETER_LIST RP STATEMENT_BLOCK {
        $$ = initFunctionDefineStatement($2, $4, $6);
    }
    ;


PARAMETER_LIST:
    IDENTIFIER {
        $$ = initParameterList($1);
    }
    |
    PARAMETER_LIST COMMA IDENTIFIER {
        $$ = $1->add($1, $3);
    }
    ;


ARGUMENT_LIST:
    OR_EXPRESSION {
        $$ = initArgumentList($1);
    }
    |
    ARGUMENT_LIST COMMA OR_EXPRESSION {
        $$ = $1->add($1, $3);
    }
    ;


EXPRESSION:
    OR_EXPRESSION
    |
    IDENTIFIER ASSIGN EXPRESSION {
//        ($1)->refer($1);
        $$ = initAssignExpression($1, $3);
    }
    ;

OR_EXPRESSION:
    AND_EXPRESSION
    |
    OR_EXPRESSION OR AND_EXPRESSION {
        $$ = initBinaryExpression(OR_OPERATOR, $1, $3);
    }
    ;

AND_EXPRESSION:
    EQUAL_EXPRESSION
    |
    AND_EXPRESSION AND EQUAL_EXPRESSION {
        $$ = initBinaryExpression(AND_OPERATOR, $1, $3);
    }
    ;

EQUAL_EXPRESSION:
    GREATER_EXPRESSION
    |
    EQUAL_EXPRESSION EQ GREATER_EXPRESSION {
        $$ = initBinaryExpression(EQ_OPERATOR, $1, $3);
    }
    |
    EQUAL_EXPRESSION NQ GREATER_EXPRESSION {
        $$ = initBinaryExpression(NQ_OPERATOR, $1, $3);
    }
    ;

GREATER_EXPRESSION:
    ADD_EXPRESSION
    |
    GREATER_EXPRESSION LT ADD_EXPRESSION {
        $$ = initBinaryExpression(LT_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION LE ADD_EXPRESSION {
        $$ = initBinaryExpression(LE_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION GT ADD_EXPRESSION {
        $$ = initBinaryExpression(GT_OPERATOR, $1, $3);
    }
    |
    GREATER_EXPRESSION GE ADD_EXPRESSION {
        $$ = initBinaryExpression(GE_OPERATOR, $1, $3);
    }
    ;

ADD_EXPRESSION:
    MUL_EXPRESSION
    |
    ADD_EXPRESSION ADD MUL_EXPRESSION {
        $$ = initBinaryExpression(ADD_OPERATOR, $1, $3);
    }
    |
    ADD_EXPRESSION SUB MUL_EXPRESSION {
        $$ = initBinaryExpression(SUB_OPERATOR, $1, $3);
    }
    ;

MUL_EXPRESSION:
    UNARY_EXPRESSION
    |
    MUL_EXPRESSION MUL UNARY_EXPRESSION {
        $$ = initBinaryExpression(MUL_OPERATOR, $1, $3);
    }
    |
    MUL_EXPRESSION DIV UNARY_EXPRESSION {
        $$ = initBinaryExpression(DIV_OPERATOR, $1, $3);
    }
    |
    MUL_EXPRESSION MOD UNARY_EXPRESSION {
        $$ = initBinaryExpression(MOD_OPERATOR, $1, $3);
    }
    ;

UNARY_EXPRESSION:
    POWER_EXPRESSION
    |
    SUB UNARY_EXPRESSION {
        $$ = initUnaryExpression(SUB_OPERATOR, $2);
    }
    |
    NOT UNARY_EXPRESSION {
        $$ = initUnaryExpression(NOT_OPERATOR, $2);
    }
    ;

POWER_EXPRESSION:
    VALUE_EXPRESSION
    |
    POWER_EXPRESSION POWER VALUE_EXPRESSION {
        $$ = initBinaryExpression(POWER_OPERATOR, $1, $3);
    }
    ;

VALUE_EXPRESSION:
    FUNCTION_CALL_EXPRESSION
    |
    VALUE {
       $$ = initValueExpression($1);
    }
    |
    LP EXPRESSION RP {
        $$ = $2;
    }
    |
    IDENTIFIER {
//        $1->refer($1);
        $$ = initVariableExpression($1);
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
    STRING
    ;

FUNCTION_CALL_EXPRESSION:
    IDENTIFIER LP RP {
//    	$1->refer($1);
	$$ = initFunctionCallExpression($1, NULL);
    }
    |
    IDENTIFIER LP ARGUMENT_LIST RP {
//    	$1->refer($1);
    	$$ = initFunctionCallExpression($1, $3);
    }
    ;

%%