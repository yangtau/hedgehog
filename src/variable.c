//
// Created by Tau on 2019/2/12.
//
#include "variable.h"
#include "oop.h"
#include "debug.h"
#include "value.h"
#include <stdlib.h>
#include "function.h"

static void freeVariable(Variable *self) {
    on_self(self->id, release);
    if (self->v.type == STRING_VALUE) on_self(self->v.v.string_value, release);
    else if (self->v.type == FUNCTION_VALUE) del(self->v.v.function);
    free(self);
}


Variable *initVariable(String *id, Value v) {
    Variable *var = malloc(sizeof(Variable));
    var->id = id;
    var->v = v;
    var->free = freeVariable;
    return var;
}
