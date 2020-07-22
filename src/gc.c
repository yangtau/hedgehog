#include "gc.h"
#include "memory.h"
#include "common.h"
#include "value.h"

typedef struct hg_object_wrapper {
    struct hg_object_wrapper* next;
    size_t size;
    bool mark;
    struct hg_object obj;
} obj_wrapper;

#define obj2wrapper_(ptr)                                       \
    ({                                                          \
        struct hg_object* _obj = (ptr);                         \
        (obj_wrapper*)((uint8_t*)_obj -                         \
                       struct_field_offset_(obj_wrapper, obj)); \
    })

static obj_wrapper* object_list = NULL;
static struct vm* vm            = NULL;
static size_t object_mem_size   = 0;

struct hg_object* gc_alloc(size_t size) {
    assert(size >= sizeof(struct hg_object));
    assert(vm != NULL);

    struct hg_object_wrapper* wrapper = hg_realloc(
        NULL, 0, size + sizeof(obj_wrapper) - sizeof(struct hg_object));

    if (wrapper != NULL) {
        object_mem_size += size;
    }

    wrapper->mark = false;
    wrapper->size = size;
    wrapper->next = object_list;
    object_list   = wrapper;

    return &wrapper->obj;
}

static void mark() {
    for (struct hg_value* p = vm->stack; p < vm->stack_top; p++) {
        if (VAL_IS_OBJ(*p)) {
            obj_wrapper* wrapper = obj2wrapper_(VAL_AS_OBJ(*p));

            wrapper->mark = true;

            switch (wrapper->obj.type) {
            case HG_OBJ_MAP:
            case HG_OBJ_SET:
            case HG_OBJ_LIST:
            case HG_OBJ_FUNCTION:
            case HG_OBJ_STRING:
            case HG_OBJ_SYMBOL:
            case HG_OBJ_TUPLE:
                break;
            }
        }
    }
}

static void sweep() {
}

#undef obj2wrapper_

