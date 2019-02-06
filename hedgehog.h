/* Created by Tau on 05/02/2019 */
#ifndef _HG_HEDGEHOG_H_
#define _HG_HEDGEHOG_H_
#include "expression.h"
#include "interpreter.h"
#include "value.h"

typedef enum {
    EXPRESSION,
    IF_STATEMENT,
    ELSE_STATEMENT,
    ELSEIF_STATEMENT,
    FOR_STATEMENT,
    BREAK_STATEMENT,
    CONTINUE_STATEMENT,
    FUNCTION_DEFINE_STATEMENT,
    RETURN_STATEMNT,
} StatementType;

typedef struct {
    StatementType type;
    union {
        Expression expression;
    } s;
} Statement;

#endif /*_HG_HEDGEHOG_H_*/