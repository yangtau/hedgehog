#include <stdlib.h>
#include "hedgehog.h"

int yyerror(char const *str) {
    extern char *yytext;
    fprintf(stderr, "--ERROR:%s, %d--\n", str, yytext[0]);
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
    Interpreter *interpreter = initInterpreter();
    interpreter->compile(interpreter, file);
    interpreter->interpret(interpreter);
    interpreter->free(interpreter);
    return 0;
}