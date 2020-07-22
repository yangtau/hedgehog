#include "gc.h"
#include "list.h"
#include "memory.h"
#include "common.h"
#include "value.h"
#include "hash.h"

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

static obj_wrapper* wrapper_list   = NULL;
static struct vm* vm               = NULL;
static size_t object_mem_size      = 0;
static size_t object_mem_size_hold = 1 << 8;

static void mark();
static void sweep();

struct hg_object* gc_alloc(size_t size) {
    assert(size >= sizeof(struct hg_object));
    assert(vm != NULL);

    if (object_mem_size >= object_mem_size_hold) {
        mark();
        sweep();

        object_mem_size_hold <<= 1;
        if (object_mem_size_hold == 0) {
            object_mem_size = 1 << 8;
        }
    }

    struct hg_object_wrapper* wrapper = hg_realloc(
        NULL, 0, size + sizeof(obj_wrapper) - sizeof(struct hg_object));

    if (wrapper != NULL) {
        object_mem_size += size;
    }

    wrapper->mark = false;
    wrapper->size = size;
    wrapper->next = wrapper_list;
    wrapper_list  = wrapper;

    return &wrapper->obj;
}

static void mark_obj(struct hg_object* obj);

static _force_inline_ void mark_list(struct hg_list* list) {
    for (size_t i = 0; i < list->arr.len; i++) {
        if (VAL_IS_OBJ(list->arr.values[i])) {
            mark_obj(VAL_AS_OBJ(list->arr.values[i]));
        }
    }
}

static _force_inline_ void mark_map(struct hash_map* map) {
    for (size_t i = 0; i < map->len; i++) {
        if (VAL_IS_OBJ(map->entries[i].key)) {
            mark_obj(VAL_AS_OBJ(map->entries[i].key));
        }
        if (VAL_IS_OBJ(map->entries[i].value)) {
            mark_obj(VAL_AS_OBJ(map->entries[i].value));
        }
    }
}

static _force_inline_ void mark_set(struct hash_set* set) {
    for (size_t i = 0; i < set->len; i++) {
        if (VAL_IS_OBJ(set->items[i])) {
            mark_obj(VAL_AS_OBJ(set->items[i]));
        }
    }
}

static void mark_obj(struct hg_object* obj) {
    obj_wrapper* wrapper = obj2wrapper_(obj);
    wrapper->mark        = true;

    switch (obj->type) {
    case HG_OBJ_MAP:
        mark_map((struct hash_map*)obj);
        break;
    case HG_OBJ_SET:
        mark_set((struct hash_set*)obj);
        break;
    case HG_OBJ_LIST:
        mark_list((struct hg_list*)obj);
        break;
    case HG_OBJ_FUNCTION:
    case HG_OBJ_STRING:
    case HG_OBJ_SYMBOL:
    case HG_OBJ_TUPLE:
        break;
    }
}

static inline void mark() {
    for (struct hg_value* p = vm->stack; p < vm->stack_top; p++) {
        if (VAL_IS_OBJ(*p)) {
            mark_obj(VAL_AS_OBJ(*p));
        }
    }
}

static inline void sweep() {
    obj_wrapper** p = &wrapper_list;

    while (*p) {
        if (!(*p)->mark) {
            obj_wrapper* t = *p;
            *p             = t->next;

            // TODO: make all object flexiable to avoid free and alloc twice for every object

            hg_obj_free_(&t->obj);
            hg_realloc(t, t->size, 0);
        } else {
            (*p)->mark = true;
        }
        p = &(*p)->next;
    }
}

void gc_clean() {
    sweep();
}

#undef obj2wrapper_
