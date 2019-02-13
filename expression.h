/* Created by Tau on 7/2/2019. */
#ifndef _HG_EXPRESSION_H_
#define _HG_EXPRESSION_H_

#include "environment.h"
#include "value.h"
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

typedef struct ExpressionTag Expression;
typedef struct ArgumentListTag ArgumentList;
typedef struct ParameterTag Parameter;
typedef struct ParameterListTag ParameterList;

struct ExpressionTag {

    void (*free)(void *self);

    Value (*evaluate)(void *self, Environment *env);

    void *next;
};


void *initVariableExpression(String *id);

void *initBinaryExpression(OperatorType operatorType,
                           Expression *left,
                           Expression *right);

void *initUnaryExpression(OperatorType operatorType,
                          Expression *expression);

void *initAssignExpression(String *id, Expression *expression);

void *initValueExpression(Value value);

void *initFunctionCallExpression(String *id, ArgumentList *args);

struct ArgumentListTag {
    Expression *head;
    Expression *tail;
    int cnt;

    ArgumentList *(*add)(ArgumentList *list, void *exp);

    void (*free)(ArgumentList *list);
};

ArgumentList *initArgumentList(Expression *head);

struct ParameterTag {
    String *name;
    Parameter *next;
};

struct ParameterListTag {
    Parameter *head;
    Parameter *tail;
    int cnt;

    ParameterList *(*add)(ParameterList *list, String *para);

    void (*free)(ParameterList *self);
};

ParameterList *initParameterList(String *head);

#endif /*_HG_EXPRESSION_H_*/

