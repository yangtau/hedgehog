#include "common.h"
#include "ast_node.h"
#include "hedgehog.h"
#include "memory.h"

extern int yyparse(struct parser_state* p);

int main(int argc, char** argv) {
    extern FILE* yyin;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
        return -1;
    }

    FILE* f;
    if ((f = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "faild to open `%s`\n", argv[1]);
        return -1;
    }

    yyin = f;

    struct parser_state p = {
        .lineno = 1, .tline = 1, .lval = NULL, .fname = argv[1]};

    while (yyparse(&p))
        ;
    ast_node_dump(p.lval, 0, stdout);
    ast_node_free(p.lval);
    assert(hg_memory_usage() == 0u);
    return 0;
}
