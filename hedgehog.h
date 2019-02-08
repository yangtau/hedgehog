/* Created by Tau on 05/02/2019 */
#ifndef _HG_HEDGEHOG_H_
#define _HG_HEDGEHOG_H_

#include "interpreter.h"
#include "value.h"
#include "debug.h"

typedef enum {
    VALUE_EXPRESSION,
    ASSIGN_EXPRESSION,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
} ExpressionType;

typedef enum {
    ADD_OPERATOR,
    SUB_OPERATOR,
    MUL_OPERATOR,
    DIV_OPERATOR,
    MOD_OPERATOR,
    POWER_OPERATOR,

    OR_OPERATOR,
    AND_OPERATOR,//7
    NOT_OPERATOR,

    EQ_OPERATOR,  //==
    NQ_OPERATOR,  //!=
    GT_OPERATOR,  //>
    LT_OPERATOR,  //<
    GE_OPERATOR,  //>=
    LE_OPERATOR,  //<=
} OperatorType;
// BINARY_EXPRESSION, UNARY_EXPRESSION,

typedef struct ExpressionTag Expression;

typedef struct {
    OperatorType operatorType;
    Expression *left, *right;
} BinaryExpression;

typedef struct {
    OperatorType operatorType;
    Expression *expression;
} UnaryExpression;

typedef struct {
    char *id;
    Expression *expression;
} AssignExpression;

struct ExpressionTag {
    ExpressionType type;
    union {
        Value value;
        AssignExpression assignExpression;
        BinaryExpression binaryExpression;
        UnaryExpression unaryExpression;
    } e;
};

Expression *createBinaryExpression(OperatorType operatorType, Expression *left,
                                   Expression *right);

Expression *createAssignExpression(char *id, Expression *expression);

Expression *createUnaryExpression(OperatorType operatorType, Expression *expression);

Expression *createValueExpression(Value value);

void freeExpression(Expression *expression);

Value evaluateExpression(Expression *expression);

typedef enum {
    EXPRESSION_STATEMENT,
    IF_STATEMENT,
    ELSE_STATEMENT,
    ELSEIF_STATEMENT,
    FOR_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    FUNCTION_DEFINE_STATEMENT,
    RETURN_STATEMENT,
} StatementType;

typedef struct {
    StatementType type;
    union {
        Expression *expression;
    } s;
} Statement;

Statement *createStatement();

#endif /*_HG_HEDGEHOG_H_*/