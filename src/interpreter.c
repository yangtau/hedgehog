/* Created by Tau on 06/02/2019 */

#include "interpreter.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "function.h"
// char to int

static void freeInterpreter(Interpreter *interpreter) {
    interpreter->globalEnv->free(interpreter->globalEnv);
    interpreter->list->free(interpreter->list);
    free(interpreter);
}

static void interpret(Interpreter *interpreter) {
    interpreter->list->execute(interpreter->list, interpreter->globalEnv);
}

static Interpreter *current_interpreter;

int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "\nERROR:%s, near '%s'\n", str, yytext);
    return 0;
}

static void compileWithStr(Interpreter *interpreter, char *str) {
    //
    current_interpreter = interpreter;

    typedef struct yy_buffer_state *YY_BUFFER_STATE;
    extern int yyparse();
    extern YY_BUFFER_STATE yy_scan_string(char *str);
    extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
    YY_BUFFER_STATE buffer = yy_scan_string(str);
    if (yyparse()) {
        fprintf(stderr, "Error!\n");
        exit(1);
    }
    yy_delete_buffer(buffer);
}

static void compile(Interpreter *interpreter, FILE *file) {
    extern int yyparse(void);
    extern FILE *yyin;
    yyin = file;
    current_interpreter = interpreter;
    if (yyparse()) {
        fprintf(stderr, "Error !\n");
        exit(1);
    }
}

static void clearStatement(Interpreter *interpreter) {
    interpreter->list = initStatementList();// TODO: free old
    // FIXME: add gc
}

Interpreter *getCurrentInterpreter() {
    return current_interpreter;
}


Interpreter *initInterpreter() {
    Interpreter *interpreter = (Interpreter *) malloc(sizeof(Interpreter));
    interpreter->list = initStatementList();
    interpreter->globalEnv = initEnvironment();
    addNativeFunction(interpreter->globalEnv);
    interpreter->free = freeInterpreter;
    interpreter->compile = compile;
    interpreter->compileWithStr = compileWithStr;
    interpreter->interpret = interpret;
    interpreter->clearStatement = clearStatement;
    return interpreter;
}
