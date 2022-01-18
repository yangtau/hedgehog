#include "common.h"
#include "ast.h"
#include "memory.h"
#include <string.h>

extern int hg_yy_parse(struct hg_parser_state* p);

extern struct yy_buffer_state* hg_yy_scan_string(const char* yystr);
extern int hg_yylex_destroy(void);

int main(int argc, char** argv) {
    struct hg_parser_state p = {
        .nerr   = 0,
        .lval   = NULL,
        .lineno = 1,
        .tline  = 1,
    };

    if (argc == 2) {
        extern FILE* hg_yyin;
        if ((hg_yyin = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "faild to open `%s`\n", argv[1]);
            return -1;
        }

        p.fname = argv[1];
        while (hg_yy_parse(&p))
            ;
        fclose(hg_yyin);
        hg_yylex_destroy();
    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        hg_yy_scan_string(argv[2]);
        while (hg_yy_parse(&p))
            ;
        hg_yylex_destroy();
    } else {
        fprintf(stderr, "Usage: %s [filename]\n\t%s -s [code]\n", argv[0],
                argv[0]);
        return -1;
    }

    // ast_node_dump(p.lval, 0, stdout);
    // ast_node_free(p.lval);
    // assert(hg_memory_usage() == 0u);
    return 0;
}
