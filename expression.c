/* Created by Tau on 7/2/2019. */
#include "expression.h"
#include <stdlib.h>
#include "debug.h"
#include "environment.h"
#include "function.h"
#include "oop.h"
#include "value.h"

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

typedef struct {
    Expression base;
    ArgumentList *args;
    ParameterList *paras;
} MultiAssignExpression;

// identifier expression
static void freeVariableExpression(Expression *_self) {
    VariableExpression *self = (VariableExpression *) _self;
    on_self(self->id, release);
    free(self);
}

static Value evaluateVariableExpression(Expression *_self, Environment *env) {
    VariableExpression *self = (VariableExpression *) _self;
    on_self(self->id, refer);
    return env->findVariable(env, self->id)->v;
}

const static Expression VariableExpressionBase = {freeVariableExpression,
                                                  evaluateVariableExpression};

void *initVariableExpression(String *id) {
    VariableExpression *exp = malloc(sizeof(VariableExpression));
    exp->base = VariableExpressionBase;
    exp->id = id;
    return exp;
}

// binary expression

static Value evaluateBinaryExpression(Expression *_self, Environment *env) {
    BinaryExpression *self = (BinaryExpression *) _self;
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

static void freeBinaryExpression(Expression *_self) {
    BinaryExpression *self = (BinaryExpression *) _self;
    del(self->left);
    del(self->right);
    free(self);
}

const static Expression BinaryExpressionBase = {freeBinaryExpression,
                                                evaluateBinaryExpression};

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

static Value evaluateAssignExpression(Expression *_self, Environment *env) {
    AssignExpression *self = (AssignExpression *) _self;
    Value value = self->expression->evaluate(self->expression, env);
    on_self(self->id, refer);
    env->addVariable(env, initVariable(self->id, value));
    return value;
}

static void freeAssignExpression(Expression *_self) {
    AssignExpression *self = (AssignExpression *) _self;
    del(self->expression);
    on_self(self->id, release);
    free(_self);
}

const static Expression AssignExpressionBase = {freeAssignExpression,
                                                evaluateAssignExpression};

void *initAssignExpression(String *id, Expression *expression) {
    //    log(("id: %s", id->str));
    AssignExpression *exp = malloc(sizeof(UnaryExpression));
    exp->expression = expression;
    exp->base = AssignExpressionBase;
    exp->id = id;
    return exp;
}

// unary expression
static Value evaluateUnaryExpression(Expression *_self, Environment *env) {
    UnaryExpression *self = (UnaryExpression *) _self;
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

void freeUnaryExpression(Expression *_self) {
    UnaryExpression *self = (UnaryExpression *) _self;
    del(self->expression);
    free(self);
}

const static Expression UnaryExpressionBase = {freeUnaryExpression,
                                               evaluateUnaryExpression};

void *initUnaryExpression(OperatorType operatorType, Expression *expression) {
    UnaryExpression *exp = malloc(sizeof(UnaryExpression));
    exp->base = UnaryExpressionBase;
    exp->operatorType = operatorType;
    exp->expression = expression;
    return exp;
}

// value Expression

static Value evaluateValueExpression(Expression *_self, Environment *env) {
    return ((ValueExpression *) _self)->value;
}

static void freeValueExpression(Expression *_self) {
    ValueExpression *self = (ValueExpression *) _self;
    if (self->value.type == STRING_VALUE) {
        on_self(self->value.v.string_value, release);
    } else if (self->value.type == FUNCTION_VALUE) {
        del(self->value.v.function);
    }
    free(self);
}

const static Expression ValueExpressionBase = {
        freeValueExpression,
        evaluateValueExpression,
};

void *initValueExpression(Value value) {
    ValueExpression *exp = malloc(sizeof(ValueExpression));
    exp->value = value;
    exp->base = ValueExpressionBase;
    return exp;
}

// function call expression

static void freeFunctionCallExpression(Expression *_self) {
    FunctionCallExpression *self = (FunctionCallExpression *) _self;
    if (self->args != NULL)
        del(self->args);
    on_self(self->id, release);
    free(self);
}

static Value evaluateFunctionExpression(Expression *_self, Environment *env) {
    FunctionCallExpression *self = (FunctionCallExpression *) _self;
    on_self(self->id, refer);
    Variable *var = env->findVariable(env, self->id);
    if (var->v.type != FUNCTION_VALUE) {
        panic(("%s is not callable", var->id->str));
    }
    FunctionDefine *func = var->v.v.function;
    return func->call(func, self->args, env);
}

const static Expression FunctionCallExpressionBase = {
        freeFunctionCallExpression, evaluateFunctionExpression};

void *initFunctionCallExpression(String *id, ArgumentList *args) {
    FunctionCallExpression *exp = malloc(sizeof(FunctionCallExpression));
    exp->base = FunctionCallExpressionBase;
    exp->id = id;
    //    on_self(id, refer);
    exp->args = args;
    return exp;
}

// argument list

static ArgumentList *addToArgumentList(ArgumentList *list, Expression *exp) {
//    list->tail->next = exp;
//    list->tail = exp;
//    //    list->tail->next = NULL;
//    list->cnt++;
    exp->pre = list->last;
    list->last = exp;
    return list;
}

static void freeArgumentList(ArgumentList *list) {
    while (list->last != NULL) {
        Expression *p = list->last->pre;
        del(list->last);
        list->last = p;
    }
    free(list);
}

ArgumentList *initArgumentList(Expression *head) {
    ArgumentList *list = malloc(sizeof(ArgumentList));
    list->last = head;
    head->pre = NULL;
//    list->head = list->tail = head;
//    list->head->next = NULL;
//    list->cnt = 1;
    list->free = freeArgumentList;
    list->add = addToArgumentList;
    return list;
}

// parameter list
static ParameterList *addToParameterList(ParameterList *list, String *id) {
    Parameter *p = malloc(sizeof(Parameter));
    p->name = id;
    p->pre = list->last;
    list->last = p;
//    p->next = list->tail->next;
//    list->tail->next = p;
//    list->tail = p;
//    list->cnt++;
    return list;
}

static void freeParameterList(ParameterList *list) {
//    log(("free parameter"));
    while (list->last != NULL) {
        Parameter *p = list->last->pre;
        on_self(list->last->name, release);
        free(list->last);
        list->last = p;
    }
    free(list);
}

ParameterList *initParameterList(String *head) {
    ParameterList *list = malloc(sizeof(ParameterList));
    Parameter *p = malloc(sizeof(Parameter));
    p->pre = NULL;
    p->name = head;
    list->last = p;
//    list->head = list->tail = p;
    list->free = freeParameterList;
    list->add = addToParameterList;
//    list->cnt = 1;
    return list;
}

//multi assign expression

static void freeMultiAssignExpression(Expression *_self) {
    MultiAssignExpression *self = (MultiAssignExpression *) _self;
    del(self->paras);
    del(self->args);
    free(self);
}


void multiAssign(ParameterList *paras, ArgumentList *args, Environment *evalEnv, Environment *varEnv) {
    if (args != NULL && paras != NULL) {
        Expression *arg = args->last;
        Parameter *p = paras->last;
        int cnt = 0;
        int len = 6;
        Value *values = calloc(sizeof(Value), len);
        while (arg != NULL) {
            on_self(p->name, refer);
            if (cnt == len) {
                len *= 2;
                values = realloc(values, sizeof(Value) * len);
            }
            values[cnt++] = arg->evaluate(arg, evalEnv);
            arg = arg->pre;
        }
        int i = 0;
        for (; i < cnt && p; i++, p = p->pre) {
            on_self(p->name, refer);
            Variable *var = initVariable(p->name, values[i]);
            varEnv->addVariable(varEnv, var);
        }
        free(values);
        if (i != cnt)
            panic(("too many arguments."));
        if (p != NULL)
            panic(("too few arguments."));
    } else if (args == NULL && paras != NULL) {
        panic(("too few arguments."));
    } else if (args != NULL && paras == NULL) {
        panic(("too many arguments."));
    }
}

static Value evaluateMultiAssignExpression(Expression *_self, Environment *env) {
    MultiAssignExpression *self = (MultiAssignExpression *) _self;
    Value v = {NULL_VALUE};
    multiAssign(self->paras, self->args, env, env);
//    if (self->args != NULL && self->paras != NULL) {
//        Expression *arg = self->args->last;
//        Parameter *p = self->paras->last;
//        for (; p != NULL && arg != NULL;) {
//            on_self(p->name, refer);
//            Variable *var = initVariable(p->name, arg->evaluate(arg, env));
//            env->addVariable(env, var);
//            p = p->pre;
//            arg = arg->pre;
//        }
//        if (arg != NULL)
//            panic(("too many arguments."));
//        if (p != NULL)
//            panic(("too few arguments."));
//    } else if (self->args == NULL && self->paras != NULL) {
//        panic(("too few arguments."));
//    } else if (self->args != NULL && self->paras == NULL) {
//        panic(("too many arguments."));
//    }
    return v;
}

static Expression MultiAssignExpressionBase = {
        freeMultiAssignExpression, evaluateMultiAssignExpression
};

void *initMultiAssignExpression(ParameterList *paras, ArgumentList *args) {
    MultiAssignExpression *expr = malloc(sizeof(MultiAssignExpression));
    expr->args = args;
    expr->paras = paras;
    expr->base = MultiAssignExpressionBase;
    return expr;
}
