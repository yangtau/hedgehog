/* Created by Tau on 10/02/2019 */
#include "function.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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


static void print_args(Expression *arg, Environment *env) {
    if (arg) {
        Value v = arg->evaluate(arg, env);
        if (arg->pre) {
            print_args(arg->pre, env);
            printf(" ");
        }
        valuePrint(v);
    }
}

static Value call_native_print(FunctionDefine *func,
                               ArgumentList *args,
                               Environment *env) {
    Value v;

    if (args != NULL) {
        Expression *arg = args->last;
        print_args(arg, env);
    }
    printf("\n");
    v.type = NULL_VALUE;
    return v;
}

static Value call_native_input(FunctionDefine *func, ArgumentList *args, Environment *env) {
    Value res;
    char buf[1024];

    if (args != NULL) {
        panic(("too many arguments"));
    }

    fgets(buf, 1024, stdin);
    buf[1023] = '\0';

    int len = strlen(buf);
    if (buf[len-1] == '\n') {
        buf[len-1] = '\0';
    }
    
    res.type = STRING_VALUE;
    res.v.string_value = initString(buf);
   
    return res;
}

static void freeNULL(FunctionDefine *_) {}

static FunctionDefine printFunction = {NULL, NULL, call_native_print, freeNULL};
static FunctionDefine inputFunction = {NULL, NULL, call_native_input, freeNULL};

void addNativeFunction(Environment *env) {
    Value v;
    v.type = FUNCTION_VALUE;
    v.v.function = &printFunction;
    env->addVariable(env, initVariable(initString("puts"), v));

    v.v.function = &inputFunction;
    env->addVariable(env, initVariable(initString("gets"), v));
}
