#ifndef _HG_EXPRESSION_H_
#define _HG_EXPRESSION_H_
#include "value.h"

typedef enum {
    VALUE_EXPRESSION,
    IDENTIFIER_EXPRESSION,
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
    AND_OPERATOR,  // 7
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
    String* id;
} IdentifierExpression;

typedef struct {
    OperatorType operatorType;
    Expression *left, *right;
} BinaryExpression;

typedef struct {
    OperatorType operatorType;
    Expression *expression;
} UnaryExpression;

typedef struct {
    String* id;
    Expression *expression;
} AssignExpression;

struct ExpressionTag {
    ExpressionType type;
    union {
        Value value;
        AssignExpression assignExpression;
        BinaryExpression binaryExpression;
        UnaryExpression unaryExpression;
        IdentifierExpression identifierExpression;
    } e;
};

void freeExpression(Expression *expression);

Value evaluateExpression(Expression *expression);

Expression *createIdentifierExpression(String* id);

Expression *createBinaryExpression(OperatorType operatorType,
                                   Expression *left,
                                   Expression *right);

Expression *createUnaryExpression(OperatorType operatorType,
                                  Expression *expression);

Expression *createAssignExpression(String* id, Expression *expression);

Expression *createValueExpression(Value value);

#endif /*_HG_EXPRESSION_H_*/