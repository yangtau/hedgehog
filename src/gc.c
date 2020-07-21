#include "gc.h"
#include "memory.h"
#include "common.h"

struct __object_wrapper {
    struct __object_wrapper* next;
    struct hg_object obj;
};

static struct __object_wrapper* object_list = NULL;
static struct vm* vm                        = NULL;
static size_t object_mem_size               = 0;

struct hg_object* gc_alloc(size_t size) {
    assert(size >= sizeof(struct hg_object));
    assert(vm != NULL);

    struct __object_wrapper* wrapper = hg_realloc(
        NULL, 0,
        size + sizeof(struct __object_wrapper) - sizeof(struct hg_object));

    wrapper->next = object_list;
    object_list   = wrapper;

    return &wrapper->obj;
}

static void gc_free(struct hg_object* obj, size_t size) {
    hg_realloc(obj - struct_field_offset_(struct __object_wrapper, obj), size,
               0);
}

static void mark() {
}

static void sweep() {
}
