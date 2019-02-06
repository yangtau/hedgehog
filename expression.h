/* Created by Tau on 06/02/2019 */
#ifndef _HG_EXPRESSION_H_
#define _HG_EXPRESSION_H_
#include "hedgehog.h"

typedef enum {
    VALUE_EXPRESSION,
    ASSIGN_EXPRESSION,
    BINARY_EXPRESSION,
    UNARY_EXPRESSION,
    FUNCTION_CALL_EXPRESSION,
} ExpressionType;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    POWER,

    OR,
    AND,
    NOT,

    ASSIGN,

    EQ,  //==
    NQ,  //!=
    GT,  //>
    LT,  //<
    GE,  //>=
    LE,  //<=
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
    Identifier *id;
    Expression *expression;
} AssignExpression;

struct ExpressionTag {
    ExpressionType type;
    union {
        Value value;
        AssignExpression assignExpression;
        BinaryExpression binaryExpression;
        UnaryExpression unaryExpressin;
    } e;
};

void createBinaryExpression(OperatorType operatorType, Expression *left,
                            Expression *right);
void createAssignExpression(Identifier *id, Expression *expression);
void createUnaryExpression(OperatorType operatorType, Expression *expression);

Value evaluateExpression(Expression *expression);

#endif /*_HG_EXPRESSION_H_*/