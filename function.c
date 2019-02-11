/* Created by Tau on 10/02/2019 */
#include "function.h"
#include <stdlib.h>
#include "debug.h"
#define del(x) (x)->free(x)
#define self(x, y) (x)->y(x)

static void addParameterToList(ParameterList* list, String* id) {
    list->tail->next = (Parameter*)malloc(sizeof(Parameter));
    list->tail = list->tail->next;
    list->tail->next = NULL;
    id->refer(id);
    list->tail->name = id;
    id->release(id);
}

static void freeParameterList(ParameterList* list) {
    while (list->head != NULL) {
        Parameter* p = list->head->next;
        free(list->head);
        list->head = p;
    }
    free(list);
}

ParameterList* initParameterList(String* head) {
    ParameterList* list = (ParameterList*)malloc(sizeof(ParameterList));
    list->head = list->tail = (Parameter*)malloc(sizeof(Parameter));
    list->tail->name = head;
    list->tail->next = NULL;
    head->refer(head);
    list->add = addParameterToList;
    list->free = freeParameterList;
    return list;
}

static void freeFunction(FunctionDefine* func) {
    // func->block->free(func->block);
    del(func->block);
    del(func->paras);
    free(func);
}

static Value callFunction(FunctionDefine* func,
                          ArgumentList* args,
                          Environment* env) {
    Environment* localEnv = initEnvironment(env);
    Expression* ep = args->head->next;
    for (Parameter* p = func->paras->head; p != NULL;
         p = p->next, ep = ep->next) {
        if (ep == NULL) {
            panic("%s", "too few arguments in function call");  // TODO: name of
                                                                // function
        }
        localEnv->addVariable(localEnv, p->name, evaluateExpression(ep, env));
    }
    if (ep != NULL) {
        panic("%s",
              "too many arguments in function call");  // TODO: name of function
    }
    func->block->execute(func->block, localEnv);
    // TODO: return statement;!!!!
}

FunctionDefine* initFunctionDefine(String* id,
                                   ParameterList* paras,
                                   StatementList* block) {
    FunctionDefine* func = (FunctionDefine*)malloc(sizeof(FunctionDefine));
    // func->id= TODO: add function to env
    
    func->block = block;
    func->paras = paras;
    func->call;
    func->free = freeFunction;
    id->release(id);
    return func;
}