//
// Created by Tau on 2019/2/11.
//

#ifndef HG_OBJECT_H
#define HG_OBJECT_H

#include "value.h"

typedef struct VariableTag {
    String *id;
    Value v;

    void (*free)(struct VariableTag *self);
} Variable;

Variable *initVariable(String *id, Value v);

#endif //HG_OBJECT_H
