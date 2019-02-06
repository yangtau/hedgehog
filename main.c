#include "interpreter.h"

int main(int argc, char **argv) {
    FILE *file=stdin;
    if (argv==2) {
        file = fopen(argv[1], "r");
        if (file==NULL) {
            printf("file %s not found", argv[1]);
            exit(1);
        }
    }
    Interpreter *interpreter = createInterpreter();
    interpret(interpreter, file);
    disposeInterpreter(interpreter);
    return 0;
}