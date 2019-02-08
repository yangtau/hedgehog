#include "value.h"
#include <stdio.h>

// char *toString(Value v) {

// }

void valuePrint(Value v) {
    switch (v.type) {
        case INT_VALUE:
            printf("%" PRId32, v.v.int_value);
            break;
        case DOUBLE_VALUE:
            printf("%lf", v.v.double_value);
            break;
        case NULL_VALUE:
            printf("null");
            break;
        case BOOL_VALUE:
            printf("%s", (v.v.bool_value.v == 0 ? "false" : "true"));
            break;
        case STRING_VALUE:
            printf("%s", v.v.string_value.str);
            break;
        default:
            break;
    }
}

Value valueAdd(Value a, Value b) {
    Value v;
    if (a.type == INT_VALUE && b.type == INT_VALUE) {
        v.type = INT_VALUE;
        v.v.int_value = a.v.int_value + b.v.int_value;
    } else if (a.type == DOUBLE_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value + b.v.double_value;
    } else if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value + b.v.int_value;
    } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.int_value + b.v.double_value;
    } else {
        v.type = NULL_VALUE;
        // TODO: error
    }
    return v;
}

Value valueSubtract(Value a, Value b) {
    Value v;
    if (a.type == INT_VALUE && b.type == INT_VALUE) {
        v.type = INT_VALUE;
        v.v.int_value = a.v.int_value - b.v.int_value;
    } else if (a.type == DOUBLE_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value - b.v.double_value;
    } else if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value - b.v.int_value;
    } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.int_value - b.v.double_value;
    } else {
        v.type = NULL_VALUE;
        // TODO: error
    }
    return v;
}

Value valueMultiply(Value a, Value b) {
    Value v;
    if (a.type == INT_VALUE && b.type == INT_VALUE) {
        v.type = INT_VALUE;
        v.v.int_value = a.v.int_value * b.v.int_value;
    } else if (a.type == DOUBLE_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value * b.v.double_value;
    } else if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.double_value * b.v.int_value;
    } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
        v.type = DOUBLE_VALUE;
        v.v.double_value = a.v.int_value * b.v.double_value;
    } else {
        v.type = NULL_VALUE;
        // TODO: error
    }
    return v;
}

Value valueDivide(Value a, Value b) {
    Value v;
    v.type = NULL_VALUE;
    if (b.type == INT_VALUE) {
        if (v.v.int_value == 0) {
//            TODO: error divided by 0
            return v;
        }
        if (a.type == INT_VALUE) {
            v.type = INT_VALUE;
            v.v.int_value = a.v.int_value / b.v.int_value;
        } else if (a.type == DOUBLE_VALUE) {
            v.type = DOUBLE_VALUE;
            v.v.double_value = a.v.double_value / b.v.int_value;
        }
    } else if (b.type == DOUBLE_VALUE) {
        if (v.v.double_value == 0.0) {
            //            TODO: error divided by 0
            return v;
        }
        if (a.type == DOUBLE_VALUE) {
            v.type = DOUBLE_VALUE;
            v.v.double_value = a.v.double_value / b.v.double_value;
        } else if (a.type == INT_VALUE) {
            v.type = DOUBLE_VALUE;
            v.v.double_value = a.v.int_value / b.v.double_value;
        }
    } else {
        // TODO: error
    }
    return v;
}

Value valueModule(Value a, Value b) {
    Value v;
    if (a.type == INT_VALUE && b.type == INT_VALUE) {
        v.type = INT_VALUE;
        v.v.int_value = a.v.int_value % b.v.int_value;
    } else {
        v.type = NULL_VALUE;
        // TODO: error
    }
    return v;
}

Value valuePower(Value a, Value b) {
    Value v;
    // TODO:
    return v;
}

Value valueNot(Value v) {
    if (v.type == BOOL_VALUE) {
        v.v.bool_value.v = !v.v.bool_value.v;
    } else {
        // TODO:error
        v.type = NULL_VALUE;
    }
    return v;
}

Value valueAnd(Value a, Value b) {
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

Value valueOr(Value a, Value b) {
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

Value valueMinus(Value v) {
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

Value valueEqual(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        v.v.bool_value.v = 0;
    } else {
        switch (a.type) {
            case NULL_VALUE:
                v.v.bool_value.v = 1;
                break;
            case BOOL_VALUE:
                v.v.bool_value.v = a.v.bool_value.v == b.v.bool_value.v;
                break;
            case INT_VALUE:
                v.v.bool_value.v = a.v.int_value == b.v.int_value;
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = a.v.double_value == b.v.double_value;
                break;
            case STRING_VALUE:

                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}

Value valueNotEqual(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        v.v.bool_value.v = 1;
    } else {
        switch (a.type) {
            case BOOL_VALUE:
                v.v.bool_value.v = a.v.bool_value.v != b.v.bool_value.v;
                break;
            case INT_VALUE:
                v.v.bool_value.v = a.v.int_value != b.v.int_value;
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = a.v.double_value != b.v.double_value;
                break;
            case STRING_VALUE:
                break;
                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}

Value valueGreater(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
            v.v.bool_value.v = a.v.double_value > b.v.int_value;
        } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
            v.v.bool_value.v = a.v.int_value > b.v.double_value;
        } else {
            v.type = NULL_VALUE;
            // TODO: error
        }
    } else {
        switch (a.type) {
            case INT_VALUE:
                v.v.bool_value.v = (a.v.int_value > b.v.int_value);
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = (a.v.double_value > b.v.double_value);
                break;
            case STRING_VALUE:
                break;
                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}

Value valueLess(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
            v.v.bool_value.v = a.v.double_value < b.v.int_value;
        } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
            v.v.bool_value.v = a.v.int_value < b.v.double_value;
        } else {
            v.type = NULL_VALUE;
            // TODO: error
        }
    } else {
        switch (a.type) {
            case INT_VALUE:
                v.v.bool_value.v = a.v.int_value < b.v.int_value;
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = a.v.double_value < b.v.double_value;
                break;
            case STRING_VALUE:
                break;
                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}

Value valueGreaterOrEqual(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        if (a.type == DOUBLE_VALUE && b.type == INT_VALUE) {
            v.v.bool_value.v = a.v.double_value >= b.v.int_value;
        } else if (a.type == INT_VALUE && b.type == DOUBLE_VALUE) {
            v.v.bool_value.v = a.v.int_value >= b.v.double_value;
        } else {
            v.type = NULL_VALUE;
            // TODO: error
        }
    } else {
        switch (a.type) {
            case INT_VALUE:
                v.v.bool_value.v = a.v.int_value >= b.v.int_value;
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = a.v.double_value >= b.v.double_value;
                break;
            case STRING_VALUE:
                break;
                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}

Value valueLessOrEqual(Value a, Value b) {
    Value v;
    v.type = BOOL_VALUE;
    if (a.type != b.type) {
        // TODO: error
        v.type = NULL_VALUE;
    } else {
        switch (a.type) {
            case INT_VALUE:
                v.v.bool_value.v = a.v.int_value <= b.v.int_value;
                break;
            case DOUBLE_VALUE:
                v.v.bool_value.v = a.v.double_value <= b.v.double_value;
                break;
            case STRING_VALUE:
                break;
                //                TODO: :(
            default:
                v.type = NULL_VALUE;
                // TODO: error
                break;
        }
    }
    return v;
}