#include "value.h"

Value add(Value a, Value b) {}
Value subtract(Value a, Value b) {}
Value multiply(Value a, Value b) {}
Value divide(Value a, Value b){

}

Value module(Value a, Value b) {

}

Value not(Value v) {
    if (v.type == BOOL_VALUE) {
        v.v.bool_value.v = !v.v.bool_value.v;

    } else {
        // TODO:error
        v.type = NULL_VALUE;
    }
    return v;
}

Value and (Value a, Value b) {
    Value v;
    if (a.type == BOOL_VALUE && b.type == BOOL_VALUE) {
        v.type = BOOL_VALUE;
        v.v.bool_value.v = (a.v.bool_value.v && b.v.bool_value.v);
    } else {
        // TODO: error
        v.type = NULL_VALUE;
    }
    return v;
}

Value or (Value a, Value b) {
    Value v;
    if (a.type == BOOL_VALUE && b.type == BOOL_VALUE) {
        v.type = BOOL_VALUE;
        v.v.bool_value.v = (a.v.bool_value.v || b.v.bool_value.v);
    } else {
        // TODO: error
        v.type = NULL_VALUE;
    }
    return v;
}

Value minus(Value v) {
    switch (v.type) {
        case INT_VALUE:
            v.v.int_value = -v.v.int_value;
            break;
        case DOUBLE_VALUE:
            v.v.double_value = -v.v.double_value;
            break;
        default:  // TODO: error
            v.type = NULL_VALUE;
            break;
    }
    return v;
}
Value equal(Value a, Value b);
Value notEqual(Value a, Value b);
Value greater(Value a, Value b);
Value less(Value a, Value b);
Value greaterOrEqual(Value a, Value b);
Value lessOrEqual(Value a, Value b);