/* Created by Tau on 06/02/2019 */
#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include <inttypes.h>

typedef enum {
    BOOL_VALUE,
    NULL_VALUE,
    INT_VALUE,
    DOUBLE_VALUE,
    STRING_VALUE,
    Object_VALUE,
} ValueType;

typedef struct {
    int8_t v;  // v==0 false
} Bool;

typedef struct {
    char *str;
} String;

typedef struct {
    char *name;
    // TODO: object detail
} Object;

typedef struct {
    ValueType type;
    union {
        Bool bool_value;
        int32_t int_value;
        double double_value;
        Object object_value;
        String string_value;
    } v;
} Value;

Value add(Value a, Value b);
Value subtract(Value a, Value b);
Value multiply(Value a, Value b);
Value divide(Value a, Value b);
Value module(Value a, Value b);
Value not(Value v);
Value and(Value a, Value b);
Value or(Value a, Value b);
Value minus(Value v);
Value equal(Value a, Value b);
Value notEqual(Value a, Value b);
Value greater(Value a,Value b);
Value less(Value a, Value b);
Value greaterOrEqual(Value a, Value b);
Value lessOrEqual(Value a, Value b);
#endif /*_HG_VALUE_H_*/
