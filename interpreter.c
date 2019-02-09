#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
// char to int

static void freeInterpreter(Interpreter* interpreter) {
    interpreter->globalEnv->free(interpreter->globalEnv);
    interpreter->list->free(interpreter->list);
    free(interpreter);
}

static void interpret(Interpreter* interpreter) {
    interpreter->list->execute(interpreter->list);
}

static Interpreter* current_interpreter;

static void compile(Interpreter* interpreter, FILE* file) {
    extern int yyparse(void);
    extern FILE* yyin;
    yyin = file;
    current_interpreter = interpreter;
    if (yyparse()) {
        fprintf(stderr, "Error !\n");
        exit(1);
    }
}

Interpreter* getCurrentInterpreter() {
    return current_interpreter;
}

Interpreter* initInterpreter() {
    Interpreter* interpreter = (Interpreter*)malloc(sizeof(Interpreter));
    interpreter->list = initStatementList();
    interpreter->globalEnv = initEnvironment(NULL);
    interpreter->free = freeInterpreter;
    interpreter->compile = compile;
    interpreter->interpret = interpret;
    return interpreter;
}
