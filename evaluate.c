#include "hedgehog.h"

static Value evaluateBinaryExpression(OperatorType operatorType, Expression *left,
                                      Expression *right) {
    log("type%d", operatorType);
    Value leftValue = evaluateExpression(left);
    log("left: type: %d", leftValue.type);
    Value rightValue = evaluateExpression(right);
    log("right: type: %d", rightValue.type);
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

static Value evaluateUnaryExpression(OperatorType operatorType, Expression *expression) {
    Value value = evaluateExpression(expression);
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

static Value evaluateAssignExpression(const char *id, Expression *expression) {
    Value value = evaluateExpression(expression);
    addVariable(getCurrentInterpreter()->globalEnv, id, value);
    return value;
}

Value evaluateExpression(Expression *expression) {
    log("type: %d", expression->type);
    switch (expression->type) {
        case VALUE_EXPRESSION:
            return expression->e.value;
        case BINARY_EXPRESSION:
            return evaluateBinaryExpression(expression->e.binaryExpression.operatorType,
                                            expression->e.binaryExpression.left,
                                            expression->e.binaryExpression.right);
        case ASSIGN_EXPRESSION:
            return evaluateAssignExpression(expression->e.assignExpression.id,
                                            expression->e.assignExpression.expression);
        case UNARY_EXPRESSION:
            return evaluateUnaryExpression(expression->e.unaryExpression.operatorType,
                                           expression->e.unaryExpression.expression);
        default:
            panic("%s", "bad case...");
            break;
    }
}