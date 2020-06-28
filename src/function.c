#include "function.h"
#include "string.h"
#include "value.h"
#include "list.h"
#include "hash.h"
#include "memory.h"

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

    // check if is an object
    if (!VAL_IS_OBJ(argv[1])) {
        error_("type %x  is not subscriptable", argv[1].type);
    }

    struct hg_object* obj = VAL_AS_OBJ(argv[1]);

    switch (obj->type) {
    case HG_OBJ_LIST: {
        struct hg_list* list = (struct hg_list*)obj;
        // check if is an integer
        if (!VAL_IS_INT(argv[2])) {
            error_("list indices must be integers, not type %x", argv[2].type);
        }
        int idx = VAL_AS_INT(argv[2]);

        value_array_set(&list->arr, argv[0], idx);
    } break;

    case HG_OBJ_MAP: {
        struct hash_map* map = (struct hash_map*)obj;

        hash_map_put(map, argv[2], argv[0]);
    } break;

    default:
        error_("object type %d is not subscriptable", obj->type);
    }

    return VAL_UNDEF();
}

static struct hg_value builtin_index_get(int argc, struct hg_value* argv) {
    // xs, idx
    //  0    1
    assert(argc == 2);
    // check if is an object
    if (!VAL_IS_OBJ(argv[0])) {
        error_("type %x  is not subscriptable", argv[0].type);
    }

    struct hg_object* obj = VAL_AS_OBJ(argv[0]);

    switch (obj->type) {
    case HG_OBJ_LIST: {
        struct hg_list* list = (struct hg_list*)obj;

        // check if is an integer
        if (!VAL_IS_INT(argv[1])) {
            error_("list indices must be integers, not type %x", argv[2].type);
        }
        int idx = VAL_AS_INT(argv[1]);

        return value_array_get(&list->arr, idx);
    }

    case HG_OBJ_MAP: {
        struct hash_map* map = (struct hash_map*)obj;

        return hash_map_get(map, argv[1]);
    }

    default:
        error_("object type %d is not subscriptable", obj->type);
    }
}

// create a hash set
static struct hg_value builtin_set(int argc, struct hg_value* argv) {
    struct hash_set* set = hg_alloc_(struct hash_set);

    hash_set_init_with_capacity(set, argc);

    for (int i = 0; i < argc; i++) {
        hash_set_put(set, argv[i]);
    }

    return VAL_OBJ(set);
}

// create a hash table
static struct hg_value builtin_map(int argc, struct hg_value* argv) {
    if (argc % 2 != 0) {
        error_("expect a even number of arguments, got %d", argc);
    }

    struct hash_map* map = hg_alloc_(struct hash_map);

    hash_map_init_with_capacity(map, argc);

    for (int i = 0; i < argc; i += 2) {
        hash_map_put(map, argv[i], argv[i + 1]);
    }

    return VAL_OBJ(map);
}

static struct hg_function funcs[] = {
    [0] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_puts},
    [1] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_list},
    [2] = {.is_builtin = true, .argc = 2, .as.builtin = builtin_index_get},
    [3] = {.is_builtin = true, .argc = 3, .as.builtin = builtin_index_set},
    [4] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_set},
    [5] = {.is_builtin = true, .argc = -1, .as.builtin = builtin_map},
};

const struct hg_function* hg_function_get_builtins(int* count) {
    // TODO: alloc but not free the memory for names of built-in functions
    funcs[0].name = VAL_OBJ(hg_symbol_copy("puts", 4));
    funcs[1].name = VAL_OBJ(hg_symbol_copy("list", 4));
    funcs[2].name = VAL_OBJ(hg_symbol_copy("index_get", 9));
    funcs[3].name = VAL_OBJ(hg_symbol_copy("index_set", 9));
    funcs[4].name = VAL_OBJ(hg_symbol_copy("set", 3));
    funcs[5].name = VAL_OBJ(hg_symbol_copy("map", 3));

    *count = sizeof(funcs) / (sizeof(struct hg_function));
    return funcs;
}
