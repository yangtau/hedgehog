#include "chunk.h"
#include "vm.h"
#include "compile.h"
#include "memory.h"

extern int yyparse(struct parser_state* p);
extern FILE* yyin;

int main(int argc, char* argv[]) {
    struct chunk chk;
    struct compiler_context ctx;
    struct vm vm;
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
        .nerr   = 0,
        .lineno = 1,
        .tline  = 1,
        .lval   = NULL,
        .fname  = argv[1],
    };

    while (yyparse(&p))
        ;

    if (p.nerr != 0) {
        goto parser_error;
    }

    chunk_init(&chk);
    compiler_context_init(&ctx);

    if ((rc = compile(&ctx, p.lval, &chk)) != 0) {
        fprintf(stderr, "compile error\n");
        goto compile_error;
    }

    chunk_disassemble(&chk);

    vm_init(&vm, &chk);
    vm_run(&vm);

    vm_free(&vm);
compile_error:
    chunk_free(&chk);
parser_error:
    ast_node_free(p.lval);
    compiler_context_free(&ctx);
    assert(hg_memory_usage() == 0u);
    return 0;
}
