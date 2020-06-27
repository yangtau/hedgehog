#include "function.h"
#include "string.h"
#include "value.h"

static struct hg_value builtin_puts(int argc, struct hg_value* argv) {
    for (int i = 0; i < argc; i++) {
        hg_value_write(argv[i], stdout, false);
        printf(" ");
    }
    printf("\n");
    return VAL_UNDEF();
}

static struct hg_function funcs[] = {
    [0] =
        {
            .is_builtin = true,
            .argc       = -1,
            .as.builtin = builtin_puts,
        },
};

const struct hg_function* hg_function_get_builtins(int* count) {
    // TODO: alloc but not free the memory for names of built-in functions
    struct hg_value puts_name = VAL_OBJ(hg_symbol_copy("puts", 4));

    funcs[0].name = puts_name;

    *count = sizeof(funcs) / (sizeof(struct hg_function));
    return funcs;
}
