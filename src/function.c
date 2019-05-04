/* Created by Tau on 10/02/2019 */
#include "function.h"
#include <stdlib.h>
#include "debug.h"
#include "environment.h"
#include "oop.h"
#include "value.h"

static void freeFunction(FunctionDefine *self) {
    log(("free function"));
    if (self->block != NULL) {
        del(self->block);
    }
    if (self->parameterList != NULL) {
        del(self->parameterList);
    }
    free(self);
}

static Value callFunction(FunctionDefine *func,
                          ArgumentList *args,
                          Environment *env) {
    Environment *localEnv = initEnvironment();
    multiAssign(func->parameterList, args, env, localEnv);
    StatementResult res;
    res.type = NORMAL_RESULT;
    if (func->block)
        res = func->block->execute(func->block, localEnv);
    Value v;
    if (res.type == RETURN_RESULT) {
        v = res.returnValue;
    } else {
        v.type = NULL_VALUE;
    }
    del(localEnv);
    return v;
}

FunctionDefine *initFunctionDefine(ParameterList *paras, StatementList *block) {
    FunctionDefine *func = (FunctionDefine *) malloc(sizeof(FunctionDefine));
    func->block = block;
    func->parameterList = paras;
    func->call = callFunction;
    func->free = freeFunction;
    // func->cnt = 1;
    return func;
}

static Value call_native_print(FunctionDefine *func,
                               ArgumentList *args,
                               Environment *env) {
    if (args != NULL) {
        Expression *arg = args->last;
        // while (arg != NULL) {
        Value v = arg->evaluate(arg, env);
        valuePrint(v);
        arg = arg->pre;
        if (arg) {
            panic(("too many arguments in function call."));
        }
    }
    // del(localEnv);
    printf("\n");
    Value v;
    v.type = NULL_VALUE;
    return v;
}

static void freeNULL(FunctionDefine *_) {}

static FunctionDefine printFunction = {NULL, NULL, call_native_print, freeNULL};

void addNativeFunction(Environment *env) {
    Value v;
    v.type = FUNCTION_VALUE;
    v.v.function = &printFunction;
    env->addVariable(env, initVariable(initString("print"), v));
}