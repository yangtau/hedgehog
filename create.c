/* Created by Tau on 7/2/2019. */
#include "hedgehog.h"
#include "stdlib.h"
#include "string.h"

Expression *createBinaryExpression(OperatorType operatorType, Expression *left,
                                   Expression *right) {
    log("operator: %d", operatorType);
    Expression *p = (Expression *) malloc(sizeof(Expression));
    p->type = BINARY_EXPRESSION;
    p->e.binaryExpression.right = right;
    p->e.binaryExpression.left = left;
    p->e.binaryExpression.operatorType = operatorType;
    return p;
}

Expression *createAssignExpression(char *id, Expression *expression) {
    log("id: %s", id);
    Expression *p = (Expression *) malloc(sizeof(Expression));
    p->type = ASSIGN_EXPRESSION;
    p->e.assignExpression.expression = expression;
    p->e.assignExpression.id = (char *) malloc((strlen(id) + 1));
    strcpy(p->e.assignExpression.id, id);
//    TODO: release id !!!
    return p;
}

Expression *createUnaryExpression(OperatorType operatorType, Expression *expression) {
    log("operator: %d", operatorType);
    Expression *p = (Expression *) malloc(sizeof(Expression));
    p->type = UNARY_EXPRESSION;
    p->e.unaryExpression.expression = expression;
    p->e.unaryExpression.operatorType = operatorType;
    return p;
}

Expression *createValueExpression(Value value) {
    log("value_type: %d", value.type);
    Expression *p = (Expression *) malloc(sizeof(Expression));
    p->type = VALUE_EXPRESSION;
    p->e.value = value;
    return p;
}

void freeExpression(Expression *expression) {
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
            free(expression->e.assignExpression.id);
            break;
        default:
            break;
    }
    free(expression);
}

Statement *createStatement() {
    Statement *p = (Statement *) malloc(sizeof(Statement));
    return p;
}

