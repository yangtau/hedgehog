/*
 * @file main.c
 * @author yangtao
 * @email yangtaojay@gmail.com
 *
 * FIXME: exit free
 *
 */

#include <stdlib.h>
#include <string.h>
#include "hedgehog.h"
#include "value.h"
#include "interface/editor.h"

static Interpreter *interpreter;

void callback(char *str) {
    interpreter->compileWithStr(interpreter, str);
    interpreter->interpret(interpreter);
    interpreter->clearStatement(interpreter);
}

void commandLine() {
    interpreter = initInterpreter();
    initEditor();
    statementEnd(callback);
    while (1)
        processKeypress();
//    interpreter->free(interpreter);
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        FILE *file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("file %s valueNot found", argv[1]);
            exit(1);
        }
        Interpreter *interpreter = initInterpreter();
        interpreter->compile(interpreter, file);
        log(("compile done"));
        interpreter->interpret(interpreter);
        log(("interpret done"));
        interpreter->free(interpreter);
    } else {
        commandLine();
    }
    return 0;
}
