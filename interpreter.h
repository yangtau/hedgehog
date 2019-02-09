/* Created by Tau on 06/02/2019 */
#ifndef _HG_INTERPRETER_H_
#define _HG_INTERPRETER_H_

#include "statement.h"
#include "environment.h"
#include <stdio.h>

typedef struct InterpreterTag {
    Environment *globalEnv;
    Statement *statement;

    void (*free)(struct InterpreterTag *);

    void (*compile)(struct InterpreterTag *, FILE *);

    void (*interpret)(struct InterpreterTag *);
} Interpreter;

Interpreter *initInterpreter();

Interpreter *getCurrentInterpreter();

#endif /*_HG_INTERPRETER_H_*/
