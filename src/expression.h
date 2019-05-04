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
    AND_OPERATOR,
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
    void (*free)(Expression *self);

    Value (*evaluate)(Expression *self, Environment *env);

    Expression *pre;//used in argument list
};

void *initVariableExpression(String *id);

void *initBinaryExpression(OperatorType operatorType,
                           Expression *left,
                           Expression *right);

void *initUnaryExpression(OperatorType operatorType, Expression *expression);

void *initAssignExpression(String *id, Expression *expression);

void *initValueExpression(Value value);

void *initFunctionCallExpression(String *id, ArgumentList *args);

void *initMultiAssignExpression(ParameterList *paras, ArgumentList *args);

void multiAssign(ParameterList *paras, ArgumentList *args, Environment *evalEnv, Environment *varEnv);

struct ArgumentListTag {
    Expression *last;  // the expression int the end of the list;

    ArgumentList *(*add)(ArgumentList *list, Expression *expr);

    void (*free)(ArgumentList *list);
};

ArgumentList *initArgumentList(Expression *head);

struct ParameterTag {
    String *name;
    Parameter *pre;
};

struct ParameterListTag {
    Parameter *last;

    ParameterList *(*add)(ParameterList *list, String *para);

    void (*free)(ParameterList *self);
};

ParameterList *initParameterList(String *head);

#endif /*_HG_EXPRESSION_H_*/
