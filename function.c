/* Created by Tau on 10/02/2019 */
#include "function.h"
#include <stdlib.h>
#include "debug.h"
#include "environment.h"
#include "oop.h"
#include "value.h"

static void freeFunction(FunctionDefine* self) {
    log(("free function"));
    //    if (self->block != NULL) {
    //        del(self->block);
    //    }
    //    for (int i = 0; i < self->parameter_cnt; i++) {
    //        on_self(self->parameters[i], release);
    //    }
    //    if (self->parameterList != NULL) {
    //        del(self->parameterList);
    //    }
    free(self);
}

static Value callFunction(FunctionDefine* func,
                          ArgumentList* args,
                          Environment* env) {
    Environment* localEnv = initEnvironment();
    // localEnv->addFather(localEnv, env);
    if (args != NULL && func->parameterList != NULL) {
        Expression* arg = args->head;
        Parameter* p = func->parameterList->head;
        for (; p != NULL && arg != NULL;) {
            on_self(p->name, refer);
            Variable* var = initVariable(p->name, arg->evaluate(arg, env));
            localEnv->addVariable(localEnv, var);
            p = p->next;
            arg = arg->next;
        }
        if (arg != NULL)
            panic(("too many arguments in function call."));
        if (p != NULL)
            panic(("too few arguments in function call."));
    } else if (args == NULL && func->parameterList != NULL) {
        panic(("too few arguments in function call."));
    } else if (args != NULL && func->parameterList == NULL) {
        panic(("too many arguments in function call."));
    }
    StatementResult res = func->block->execute(func->block, localEnv);
    Value v;
    if (res.type == RETURN_RESULT) {
        v = res.returnValue;
    } else {
        v.type = NULL_VALUE;
    }
    del(localEnv);
    return v;
}

FunctionDefine* initFunctionDefine(ParameterList* paras, StatementList* block) {
    FunctionDefine* func = (FunctionDefine*)malloc(sizeof(FunctionDefine));
    func->block = block;
    func->parameterList = paras;
    func->call = callFunction;
    func->free = freeFunction;
    return func;
}

static Value call_native_print(FunctionDefine* func,
                               ArgumentList* args,
                               Environment* env) {
    // Environment* localEnv = initEnvironment();
    // localEnv->addFather(localEnv, env);
    if (args != NULL) {
        Expression* arg = args->head;
        // while (arg != NULL) {
        Value v = arg->evaluate(arg, env);
        valuePrint(v);
        // arg = arg->next;
        // if (arg)
        //     printf(", ");
        // }
        if (!arg) {
            panic(("too many arguments in function call."));
        }
    }
    // del(localEnv);
    printf("\n");
    Value v;
    v.type = NULL_VALUE;
    return v;
}

static void freeNULL(FunctionDefine* _) {}

static FunctionDefine printFunction = {NULL, NULL, call_native_print, freeNULL};

void addNativeFunction(Environment* env) {
    Value v;
    v.type = FUNCTION_VALUE;
    v.v.function = &printFunction;
    env->addVariable(env, initVariable(initString("print"), v));
}