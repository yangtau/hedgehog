#include "function.h"
#include "string.h"
#include "value.h"
#include "list.h"

static struct hg_value builtin_puts(int argc, struct hg_value* argv) {
    for (int i = 0; i < argc; i++) {
        hg_value_write(argv[i], stdout, false);
        printf(" ");
    }
    printf("\n");
    return VAL_UNDEF();
}

static struct hg_value builtin_list(int argc, struct hg_value* argv) {
    struct hg_list* list = hg_list_new(argc, argv);
    return VAL_OBJ(list);
}

static struct hg_value builtin_index_set(int argc, struct hg_value* argv) {
    // val, xs, idx
    //  0    1    2
    assert(argc == 3);
    struct hg_list* list = (struct hg_list*)VAL_AS_OBJ(argv[1]);
    int idx              = VAL_AS_INT(argv[2]);
    struct hg_value val  = argv[0];

    value_array_set(&list->arr, val, idx);
    return VAL_UNDEF();
}

static struct hg_value builtin_index_get(int argc, struct hg_value* argv) {
    // xs, idx
    //  0    1
    assert(argc == 2);
    struct hg_list* list = (struct hg_list*)VAL_AS_OBJ(argv[0]);
    int idx              = VAL_AS_INT(argv[1]);

    return value_array_get(&list->arr, idx);
}

static struct hg_function funcs[] = {
    [0] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_puts},
    [1] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_list},
    [2] = {.is_builtin = true, .argc = 2, .as.builtin = builtin_index_get},
    [3] = {.is_builtin = true, .argc = 3, .as.builtin = builtin_index_set},
};

const struct hg_function* hg_function_get_builtins(int* count) {
    // TODO: alloc but not free the memory for names of built-in functions
    funcs[0].name = VAL_OBJ(hg_symbol_copy("puts", 4));
    funcs[1].name = VAL_OBJ(hg_symbol_copy("list", 4));
    funcs[2].name = VAL_OBJ(hg_symbol_copy("index_get", 9));
    funcs[3].name = VAL_OBJ(hg_symbol_copy("index_set", 9));

    *count = sizeof(funcs) / (sizeof(struct hg_function));
    return funcs;
}
