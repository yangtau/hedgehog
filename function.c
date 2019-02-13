/* Created by Tau on 10/02/2019 */
#include "function.h"
#include <stdlib.h>
#include "debug.h"
#include "oop.h"
#include "environment.h"
#include "value.h"

static void freeFunction(FunctionDefine *self) {
    del(self->block);
    for (int i = 0; i < self->parameter_cnt; i++) {
        on_self(self->parameters[i], release);
    }
    free(self);
}


static Value callFunction(FunctionDefine *func,
                          ArgumentList *args,
                          Environment *env) {
    Environment *localEnv = initEnvironment();
    localEnv->addFather(localEnv, env);
    if (args != NULL) {
        if (func->parameter_cnt != args->cnt) {
            panic("too few or too many arguments in function call.%s", "");
        }
        Expression *arg = args->head;
        for (int i = 0; i < func->parameter_cnt; i++) {
            on_self(func->parameters[i], refer);
            Variable *var = initVariable(func->parameters[i], arg->evaluate(arg, env));
            localEnv->addVariable(localEnv, var);
            arg = arg->next;
        }
    } else if (func->parameter_cnt != 0) {
        panic("too few arguments in function call.%s", "");
    }
    StatementResult res = func->block->execute(func->block, localEnv);
    Value v;
    if (res.type == RETURN_RESULT) {
        v = res.returnValue;
    } else {
        v.type = NULL_VALUE;
    }
    return v;
}

FunctionDefine *initFunctionDefine(
        ParameterList *paras,
        StatementList *block) {
    FunctionDefine *func = (FunctionDefine *) malloc(sizeof(FunctionDefine));
    func->block = block;
    if (paras == NULL) func->parameter_cnt = 0;
    else {
        func->parameter_cnt = paras->cnt;
        func->parameters = calloc(sizeof(String *), func->parameter_cnt);
        Parameter *p = paras->head;
        size_t cnt = 0;
        while (p != NULL) {
            func->parameters[cnt++] = p->name;
            on_self(p->name, refer);
            p = p->next;
        }
    }
    func->call = callFunction;
    func->free = freeFunction;
    return func;
}

static Value call_native_print(FunctionDefine *func,
                               ArgumentList *args,
                               Environment *env) {
    Environment *localEnv = initEnvironment();
//    localEnv->addFather(localEnv, env);
    if (args != NULL) {
        Expression *arg = args->head;
        while (arg != NULL) {
            Value v = arg->evaluate(arg, env);
            valuePrint(v);
            arg = arg->next;
            if (arg) printf(", ");
        }
    }
    printf("\n");
    Value v;
    v.type = NULL_VALUE;
    return v;
}

static void freeNULL(FunctionDefine *_) {}

static FunctionDefine printFunction = {
        NULL, -1, NULL, call_native_print, freeNULL
};

void addNativeFunction(Environment *env) {
    Value v;
    v.type = FUNCTION_VALUE;
    v.v.function = &printFunction;
    env->addVariable(env, initVariable(initString("print"), v));
}