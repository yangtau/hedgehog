/* Created by Tau on 7/2/2019. */
#include <stdlib.h>
#include "expression.h"
#include "debug.h"
#include "environment.h"
#include "value.h"
#include "function.h"
#include "oop.h"

typedef struct {
    Expression base;
    String *id;
} VariableExpression;

typedef struct {
    Expression base;
    OperatorType operatorType;
    Expression *left, *right;
} BinaryExpression;

typedef struct {
    Expression base;
    OperatorType operatorType;
    Expression *expression;
} UnaryExpression;

typedef struct {
    Expression base;
    String *id;
    Expression *expression;
} AssignExpression;

typedef struct {
    Expression base;
    Value value;
} ValueExpression;

typedef struct {
    Expression base;
    String *id;
    ArgumentList *args;
} FunctionCallExpression;


// identifier expression
static void freeVariableExpression(void *_self) {
    VariableExpression *self = _self;
//    log(("free %s %d:", self->id->str, self->id->cnt));
    on_self(self->id, release);
    free(self);
}

static Value evaluateVariableExpression(void *_self, Environment *env) {
    VariableExpression *self = _self;
    on_self(self->id, refer);
    return env->findVariable(env, self->id)->v;
}

const static Expression VariableExpressionBase = {
        freeVariableExpression, evaluateVariableExpression
};

void *initVariableExpression(String *id) {
    VariableExpression *exp = malloc(sizeof(VariableExpression));
    exp->base = VariableExpressionBase;
    exp->id = id;
    return exp;
}

// binary expression

static Value evaluateBinaryExpression(void *_self,
                                      Environment *env) {
    BinaryExpression *self = _self;
    log(("type:%d", self->operatorType));
    Value leftValue = self->left->evaluate(self->left, env);
    Value rightValue = self->right->evaluate(self->right, env);
    switch (self->operatorType) {
        case ADD_OPERATOR:
            return valueAdd(leftValue, rightValue);
        case SUB_OPERATOR:
            return valueSubtract(leftValue, rightValue);
        case MUL_OPERATOR:
            return valueMultiply(leftValue, rightValue);
        case DIV_OPERATOR:
            return valueDivide(leftValue, rightValue);
        case MOD_OPERATOR:
            return valueModule(leftValue, rightValue);
        case POWER_OPERATOR:
            return valuePower(leftValue, rightValue);
        case OR_OPERATOR:
            return valueOr(leftValue, rightValue);
        case AND_OPERATOR:
            return valueAnd(leftValue, rightValue);
        case EQ_OPERATOR:
            return valueEqual(leftValue, rightValue);
        case NQ_OPERATOR:
            return valueNotEqual(leftValue, rightValue);
        case GT_OPERATOR:
            return valueGreater(leftValue, rightValue);
        case LT_OPERATOR:
            return valueLess(leftValue, rightValue);
        case GE_OPERATOR:
            return valueGreaterOrEqual(leftValue, rightValue);
        case LE_OPERATOR:
            return valueLessOrEqual(leftValue, rightValue);
        default:
            panic(("bad case..."));
    }
}

static void freeBinaryExpression(void *_self) {
    BinaryExpression *self = _self;
    del(self->left);
    del(self->right);
    free(self);
}

const static Expression BinaryExpressionBase = {
        freeBinaryExpression, evaluateBinaryExpression
};

void *initBinaryExpression(OperatorType operatorType,
                           Expression *left,
                           Expression *right) {
//    log(("operator: %d", operatorType));
    BinaryExpression *exp = malloc(sizeof(BinaryExpression));
    exp->base = BinaryExpressionBase;
    exp->right = right;
    exp->left = left;
    exp->operatorType = operatorType;
    return exp;
}

// assignment expression

static Value evaluateAssignExpression(void *_self,
                                      Environment *env) {
    AssignExpression *self = _self;
    Value value = self->expression->evaluate(self->expression, env);
    on_self(self->id, refer);
    env->addVariable(env, initVariable(self->id, value));
    return value;
}

static void freeAssignExpression(void *_self) {
    AssignExpression *self = _self;
    del(self->expression);
    on_self(self->id, release);
    free(_self);
}

const static Expression AssignExpressionBase = {
        freeAssignExpression, evaluateAssignExpression
};

void *initAssignExpression(String *id, Expression *expression) {
//    log(("id: %s", id->str));
    AssignExpression *exp = malloc(sizeof(UnaryExpression));
    exp->expression = expression;
    exp->base = AssignExpressionBase;
    exp->id = id;
    return exp;
}


// unary expression
static Value evaluateUnaryExpression(void *_self,
                                     Environment *env) {
    UnaryExpression *self = _self;
    Value value = self->expression->evaluate(self->expression, env);
    switch (self->operatorType) {
        case SUB_OPERATOR:
            return valueMinus(value);
        case NOT_OPERATOR:
            return valueNot(value);
        default:
            panic(("bad case..."));
            break;
    }
}

void freeUnaryExpression(void *_self) {
    UnaryExpression *self = _self;
    del(self->expression);
    free(self);
}

const static Expression UnaryExpressionBase = {
        freeUnaryExpression, evaluateUnaryExpression
};

void *initUnaryExpression(OperatorType operatorType,
                          Expression *expression) {
//    log(("operator: %d", operatorType));
    UnaryExpression *exp = malloc(sizeof(UnaryExpression));
    exp->base = UnaryExpressionBase;
    exp->operatorType = operatorType;
    exp->expression = expression;
    return exp;
}

//value Expression

static Value evaluateValueExpression(void *_self, Environment *env) {
    return ((ValueExpression *) _self)->value;
}

static void freeValueExpression(void *_self) {
    ValueExpression *self = _self;
    if (self->value.type == STRING_VALUE) {
        on_self(self->value.v.string_value, release);
    } else if (self->value.type == FUNCTION_VALUE) {
        del(self->value.v.function);
    }
    free(_self);
}

const static Expression ValueExpressionBase = {
        freeValueExpression, evaluateValueExpression,
};

void *initValueExpression(Value value) {
    ValueExpression *exp = malloc(sizeof(ValueExpression));
    exp->value = value;
    exp->base = ValueExpressionBase;
    return exp;
}

// function call expression

static void freeFunctionCallExpression(void *_self) {
    FunctionCallExpression *self = _self;
    if (self->args != NULL)
        del(self->args);
    on_self(self->id, release);
    free(self);
}


static Value evaluateFunctionExpression(void *_self, Environment *env) {
    FunctionCallExpression *self = _self;
    on_self(self->id, refer);
    Variable *var = env->findVariable(env, self->id);
    if (var->v.type != FUNCTION_VALUE) {
        panic(("%s is not callable", var->id->str));
    }
    FunctionDefine *func = var->v.v.function;
    return func->call(func, self->args, env);
}

const static Expression FunctionCallExpressionBase = {
        freeFunctionCallExpression, evaluateFunctionExpression
};

void *initFunctionCallExpression(String *id, ArgumentList *args) {
    FunctionCallExpression *exp = malloc(sizeof(FunctionCallExpression));
    exp->base = FunctionCallExpressionBase;
    exp->id = id;
//    on_self(id, refer);
    exp->args = args;
    return exp;
}



// argument list

static ArgumentList *addToArgumentList(ArgumentList *list, void *exp) {
    list->tail->next = exp;
    list->tail = exp;
//    list->tail->next = NULL;
    list->cnt++;
    return list;
}

static void freeArgumentList(ArgumentList *list) {
    while (list->head != NULL) {
        Expression *p = list->head->next;
        del(list->head);
        list->head = p;
    }
    free(list);
}


ArgumentList *initArgumentList(Expression *head) {
    ArgumentList *list = malloc(sizeof(ArgumentList));
    list->head = list->tail = head;
    list->head->next = NULL;
    list->cnt = 1;
    list->free = freeArgumentList;
    list->add = addToArgumentList;
    return list;
}

// parameter list
static ParameterList *addToParameterList(ParameterList *list, String *id) {
    Parameter *p = malloc(sizeof(Parameter));
    p->name = id;
    p->next = list->tail->next;
    list->tail->next = p;
    list->tail = p;
    list->cnt++;
    return list;
//    id->refer(id);
//    id->release(id);
}

static void freeParameterList(ParameterList *list) {
    log(("free parameter"));
    while (list->head != NULL) {
        Parameter *p = list->head->next;
        on_self(list->head->name, release);
        free(list->head);
        list->head = p;
    }
    free(list);
}


ParameterList *initParameterList(String *head) {
    ParameterList *list = malloc(sizeof(ParameterList));
    Parameter *p = malloc(sizeof(Parameter));
    p->next = NULL;
    p->name = head;
    list->head = list->tail = p;
//    head->refer(head);
    list->free = freeParameterList;
    list->add = addToParameterList;
    list->cnt = 1;
    return list;
}
