#include "function.h"
#include "common.h"

static uint32_t hg_function_hash(struct hg_object* _func) {
    assert(_func->type == HG_OBJ_FUNCTION);
    unimplemented_("");
}

static bool hg_function_equal(struct hg_object* _a, struct hg_object* _b) {
    unimplemented_("functions cannot be compared");
}

static void hg_function_free(struct hg_object* _func) {
}

static int hg_function_write(struct hg_object* _func, FILE* fp) {
    assert(_func->type == HG_OBJ_FUNCTION);
    struct hg_function* func = (struct hg_function*)_func;
    fprintf(fp, "%s", "function");
    return 0;
}

static struct hg_object_funcs hg_function_funcs = {
    .hash  = hg_function_hash,
    .free  = hg_function_free,
    .write = hg_function_write,
    .equal = hg_function_equal,
};

void hg_function_init(struct hg_function* func, int argc, uint8_t* addr) {
    func->argc = argc;
    func->obj  = (struct hg_object){
        .type  = HG_OBJ_FUNCTION,
        .funcs = &hg_function_funcs,
        .hash  = 0u,
    };
    func->addr = addr;
}
