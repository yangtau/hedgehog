#include "chunk.h"
#include "vm.h"
#include "compile.h"
#include "hedgehog.h"
#include "memory.h"

extern int yyparse(struct parser_state* p);
extern FILE* yyin;

int main(int argc, char* argv[]) {
    struct chunk chk;
    struct compile_state state;
    int rc = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        return -1;
    }

    if ((yyin = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "faild to open `%s`\n", argv[1]);
        return -1;
    }

    struct parser_state p = {
        .lineno = 1, .tline = 1, .lval = NULL, .fname = argv[1]};

    while (yyparse(&p))
        ;

    chunk_init(&chk);
    compile_state_init(&state);

    if ((rc = compile(p.lval, &chk, &state)) != 0) {
        fprintf(stderr, "compile error\n");
        goto compile_error;
    }

    chunk_disassemble(&chk);

compile_error:
    ast_node_free(p.lval);
    chunk_free(&chk);
    compile_state_free(&state);
    assert(hg_memory_usage() == 0u);
    return 0;
}
