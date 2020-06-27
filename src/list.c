#include "list.h"
#include "memory.h"

static void hg_list_free(struct hg_object*);
static bool hg_list_equal(struct hg_object*, struct hg_object*);
static uint32_t hg_list_hash(struct hg_object*);
static int hg_list_write(struct hg_object*, FILE*);
static struct hg_object* hg_list_add(struct hg_object*, struct hg_object*);

static struct hg_object_funcs hg_list_funcs = {
    .free  = hg_list_free,
    .equal = hg_list_equal,
    .hash  = hg_list_hash,
    .write = hg_list_write,
    .add   = hg_list_add,
};

struct hg_list* hg_list_new(int argc, struct hg_value* argv) {
    struct hg_list* list = hg_alloc_(struct hg_list);

    list->obj = (struct hg_object){
        .type  = HG_OBJ_LIST,
        .funcs = &hg_list_funcs,
    };

    value_array_init_with_values(&list->arr, argc, argv);

    return list;
}

static int hg_list_write(struct hg_object* _list, FILE* fp) {
    struct hg_list* list = (struct hg_list*)_list;

    fprintf(fp, "[");
    for (size_t i = 0; i < list->arr.len; i++) {
        hg_value_write(list->arr.values[i], fp, true);
        fprintf(fp, "%s", i == list->arr.len - 1 ? "" : ", ");
    }
    fprintf(fp, "]");
    return 0;
}

static void hg_list_free(struct hg_object* _list) {
    struct hg_list* list = (struct hg_list*)_list;

    value_array_free(&list->arr);
    hg_free_(struct hg_list, list);
}

static bool hg_list_equal(struct hg_object* _a, struct hg_object* _b) {
    return false;
}

static uint32_t hg_list_hash(struct hg_object* _a) {
    return 0u;
}

static struct hg_object* hg_list_add(struct hg_object* _a,
                                     struct hg_object* _b) {
    return NULL;
}
