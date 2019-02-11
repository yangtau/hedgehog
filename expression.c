/* Created by Tau on 7/2/2019. */
#include "expression.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "environment.h"
#include "interpreter.h"

static Expression* initExpression() {
    Expression* p = (Expression*)malloc(sizeof(Expression));
    return p;
}

Expression* createIdentifierExpression(String* id) {
    log("id: %s", id->str);
    Expression* p = initExpression();
    p->type = IDENTIFIER_EXPRESSION;
    // id.refer(id);
    p->e.identifierExpression.id = id;
    // id.release(id);
    return p;
}

Expression* createBinaryExpression(OperatorType operatorType,
                                   Expression* left,
                                   Expression* right) {
    log("operator: %d", operatorType);
    Expression* p = initExpression();
    p->type = BINARY_EXPRESSION;
    p->e.binaryExpression.right = right;
    p->e.binaryExpression.left = left;
    p->e.binaryExpression.operatorType = operatorType;
    return p;
}

Expression* createAssignExpression(String* id, Expression* expression) {
    log("id: %s", id->str);
    Expression* p = initExpression();
    p->type = ASSIGN_EXPRESSION;
    p->e.assignExpression.expression = expression;
    p->e.assignExpression.id = id;
    // id.refer(id);
    // id.release(id);
    return p;
}

Expression* createUnaryExpression(OperatorType operatorType,
                                  Expression* expression) {
    log("operator: %d", operatorType);
    Expression* p = initExpression();
    p->type = UNARY_EXPRESSION;
    p->e.unaryExpression.expression = expression;
    p->e.unaryExpression.operatorType = operatorType;
    return p;
}

Expression* createValueExpression(Value value) {
    log("value_type: %d", value.type);
    Expression* p = initExpression();
    p->type = VALUE_EXPRESSION;
    p->e.value = value;
    return p;
}

void freeExpression(Expression* expression) {
    log("type: %d", expression->type);
    switch (expression->type) {
        case VALUE_EXPRESSION:
            break;
        case BINARY_EXPRESSION:
            free(expression->e.binaryExpression.left);
            free(expression->e.binaryExpression.right);
            break;
        case UNARY_EXPRESSION:
            free(expression->e.unaryExpression.expression);
            break;
        case ASSIGN_EXPRESSION:
            free(expression->e.assignExpression.expression);
            expression->e.assignExpression.id->release(
                expression->e.assignExpression.id);
            break;
        case IDENTIFIER_EXPRESSION:
            expression->e.identifierExpression.id->release(
                expression->e.identifierExpression.id);
            break;
        default:
            panic("%s", "bad case..");
            break;
    }
    free(expression);
}

static Value evaluateBinaryExpression(OperatorType operatorType,
                                      Expression* left,
                                      Expression* right,
                                      Environment* env) {
    log("type%d", operatorType);
    Value leftValue = evaluateExpression(left, env);
    Value rightValue = evaluateExpression(right, env);
    switch (operatorType) {
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
            panic("%s", "bad case...");
    }
}

static Value evaluateUnaryExpression(OperatorType operatorType,
                                     Expression* expression,
                                     Environment* env) {
    Value value = evaluateExpression(expression, env);
    switch (operatorType) {
        case SUB_OPERATOR:
            return valueMinus(value);
        case NOT_OPERATOR:
            return valueNot(value);
        default:
            panic("%s", "bad case...");
            break;
    }
}

static Value evaluateAssignExpression(String* id,
                                      Expression* expression,
                                      Environment* env) {
    Value value = evaluateExpression(expression, env);
    env->addVariable(env, id, value);
    // id.refer(id);
    // id.release(id);
    return value;
}

static Value evaluateIdentifierExpression(String* id, Environment* env) {
    id->refer(id);
    return env->findVariable(env, id);
}

Value evaluateExpression(Expression* expression, Environment* env) {
    log("type: %d", expression->type);
    switch (expression->type) {
        case VALUE_EXPRESSION:
            return expression->e.value;
        case BINARY_EXPRESSION:
            return evaluateBinaryExpression(
                expression->e.binaryExpression.operatorType,
                expression->e.binaryExpression.left,
                expression->e.binaryExpression.right, env);
        case ASSIGN_EXPRESSION:
            expression->e.assignExpression.id->refer(
                expression->e.assignExpression.id);
            return evaluateAssignExpression(
                expression->e.assignExpression.id,
                expression->e.assignExpression.expression, env);
        case UNARY_EXPRESSION:
            return evaluateUnaryExpression(
                expression->e.unaryExpression.operatorType,
                expression->e.unaryExpression.expression, env);
        case IDENTIFIER_EXPRESSION:
            return evaluateIdentifierExpression(
                expression->e.identifierExpression.id, env);
        default:
            panic("%s", "bad case...");
            break;
    }
}

static void addExpressionToList(ArgumentList* list, Expression* exp) {
    exp->next = NULL;
    list->tail->next = exp;
    list->tail = exp;
}

static void freeArgumentList(ArgumentList* list) {
    while (list->head->next != NULL) {
        Expression* p = list->head->next;
        list->head->next = p->next;
        freeExpression(p);
    }
    free(list->head);
    free(list);
}

ArgumentList* initArgumentList() {
    ArgumentList* list = (ArgumentList*)malloc(sizeof(ArgumentList));
    list->head = (Expression*)malloc(sizeof(Expression));
    list->tail = list->head;
    list->add = addExpressionToList;
    list->free = freeArgumentList;
    list->tail->next = NULL;
    return list;
}