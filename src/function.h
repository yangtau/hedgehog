/* Created by Tau on 10/02/2019 */
#ifndef _HG_FUNCTION_H_
#define _HG_FUNCTION_H_

#include "statement.h"

struct FunctionDefineTag {

    ParameterList *parameterList;
    StatementList *block;

    Value (*call)(FunctionDefine *self,
                  ArgumentList *args,
                  Environment *env);

    void (*free)(FunctionDefine *self);
};

FunctionDefine *initFunctionDefine(
        ParameterList *paras,
        StatementList *block);

void addNativeFunction(Environment *env);

#endif /*_HG_FUNCTION_H_*/