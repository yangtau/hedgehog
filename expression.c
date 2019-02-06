#include "expression.h"

Value evaluateBinaryExpression(BinaryExpression *expression) {
    Value left, right;
    if (expression->left->type != VALUE_EXPRESSION)
        left = evaluateExpression(expression->left);
    else
        left = expression->left->e.value;
    if (expression->right->type != VALUE_EXPRESSION)
        right = evaluateExpression(expression->right);
    else
        right = expression->right->e.value;
    Value v;
    // TODO: not support operation between different type so far
    switch (expression->operatorType) {
        case ADD:
            // v = ;
            break;
        case ASSIGN:
        case NOT:
        default:
            v.type = NULL_VALUE;
            break;
    }
    return v;
}

Value evaluateUnaryExpression(UnaryExpression *expression) {}
Value evaluateAssignExpression(AssignExpression *expression) {}

Value evaluateExpression(Expression *expression) {
    switch (expression->type) {
        case VALUE_EXPRESSION:
            return expression->e.value;
        case BINARY_EXPRESSION:
            return evaluateBinaryExpression(expression);
        case ASSIGN_EXPRESSION:
            return evaluateAssignExpression(expression);
        case UNARY_EXPRESSION:
            return evaluateUnaryExpression(expression);
        default:
            // TODO: error
            Value v;
            v.type = NULL_VALUE;
            return v;
    }
}