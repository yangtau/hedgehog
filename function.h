/* Created by Tau on 10/02/2019 */
#ifndef _HG_FUNCTION_H_
#define _HG_FUNCTION_H_
#include "expression.h"
#include "statement.h"
#include "value.h"

typedef struct FunctionDefineTag FunctionDefine;
typedef struct ParameterTag Parameter;
typedef struct ParameterListTag ParameterList;

struct ParameterTag {
    String* name;
    Parameter* next;
};

struct ParameterListTag {
    Parameter *head;
    Parameter* tail;
    void (*add)(Parameter* self, String* p);
    void (*free)(Parameter* self);
};

ParameterList* initParameterList(String* head);


struct FunctionDefineTag {
    ParameterList* paras;
    StatementList* block;
    void (*free)(FunctionDefine* func);
    Value (*call)(FunctionDefine* func, ArgumentList* list, Environment* env);
};

FunctionDefine* initFunctionDefine(String* id,
                                   ParameterList* paras,
                                   StatementList* block);

#endif /*_HG_FUNCTION_H_*/