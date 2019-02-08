#include <stdlib.h>
#include "interpreter.h"

int yyerror(char const *str) {
    fprintf(stderr, "error %s ", str);
    return 0;
}

int main(int argc, char **argv) {
    FILE *file = stdin;
    if (argc == 2) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            printf("file %s valueNot found", argv[1]);
            exit(1);
        }
    }
    Interpreter *interpreter = createInterpreter();
    interpret(interpreter, file);
    disposeInterpreter(interpreter);
    return 0;
}