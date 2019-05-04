/* Created by Tau on 06/02/2019 */
#ifndef _HG_INTERPRETER_H_
#define _HG_INTERPRETER_H_

#include <stdio.h>
#include "environment.h"
#include "statement.h"

typedef struct InterpreterTag Interpreter;

struct InterpreterTag {
    Environment *globalEnv;
    StatementList *list;

    void (*free)(Interpreter *);

    void (*compile)(Interpreter *, FILE *);

    void (*compileWithStr)(Interpreter *, char *);

    void (*interpret)(Interpreter *);

    void (*clearStatement)(Interpreter *);
};

Interpreter *initInterpreter();

Interpreter *getCurrentInterpreter();

#endif /*_HG_INTERPRETER_H_*/
