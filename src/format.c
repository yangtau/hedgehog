#include "common.h"
#include "ast.h"
#include "string.h"

#include <string.h>

extern int hg_yylex_init(void** scanner);
extern int hg_yylex_destroy(void* yyscanner);
extern void* hg_yy_scan_string(const char* yy_str, void* yyscanner);
extern void hg_yyset_in(FILE* _in_str, void* yyscanner);
extern int hg_yyparse(struct hg_parser* p, void* scanner);

int main(int argc, char** argv) {
    int ret       = 0;
    void* scanner = NULL;

    hg_string_init();
    assert(hg_yylex_init(&scanner) == 0);

    struct hg_parser p = {
        .fname = NULL, .nerr = 0, .lineno = 1, .tline = 1, .ast = NULL};

    if (argc == 2) {
        FILE* yyin;
        if ((yyin = fopen(argv[1], "r")) == NULL) {
            fprintf(stderr, "faild to open `%s`\n", argv[1]);
            ret = -1;
            goto clean;
        }

        p.fname = argv[1];
        hg_yyset_in(yyin, scanner);
        while (hg_yyparse(&p, scanner))
            ;
        fclose(yyin);

        hg_ast_node_free(p.ast);
    } else if (argc == 3 && strcmp(argv[1], "-e") == 0) {
        assert(hg_yy_scan_string(argv[2], scanner));
        while (hg_yyparse(&p, scanner))
            ;

        hg_ast_node_free(p.ast);
    } else {
        fprintf(stderr, "Usage: %s [filename]\n\t%s -s [code]\n", argv[0],
                argv[0]);
    }

clean:
    hg_string_free();
    assert(hg_yylex_destroy(scanner) == 0);
    return ret;
}
