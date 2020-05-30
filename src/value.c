#include "value.h"
#include "memory.h"

//> hg_value
void hg_value_free(struct hg_value value) {
    if (value.type == HG_VALUE_OBJECT) {
        struct hg_object* obj = VAL_AS_OBJ(value);
        obj->funcs->free(obj);
    }
}

void hg_value_write(struct hg_value a, FILE* fp) {
    switch (a.type) {
    case HG_VALUE_INT:
        fprintf(fp, "%ld", VAL_AS_INT(a));
        break;
    case HG_VALUE_FLOAT:
        fprintf(fp, "%lf", VAL_AS_FLOAT(a));
        break;
    case HG_VALUE_BOOL:
        fprintf(fp, "%s", VAL_AS_BOOL(a) ? "true" : "false");
        break;
    case HG_VALUE_NIL:
        fprintf(fp, "nil");
        break;
    case HG_VALUE_OBJECT: {
        struct hg_object* obj = VAL_AS_OBJ(a);
        obj->funcs->write(obj, fp);
    } break;
    default:
        unimplemented_("type :%x\n", a.type);
    }
}

bool hg_value_equal(struct hg_value a, struct hg_value b) {
    if (a.type != b.type)
        return false;
    if (a.as._int == b.as._int)
        return true;
    if (a.type == HG_VALUE_OBJECT) {
        struct hg_object* obj_a = VAL_AS_OBJ(a);
        struct hg_object* obj_b = VAL_AS_OBJ(a);
        return obj_a->funcs->equal(obj_a, obj_b);
    }
    return false;
    /*
#define equal_as(type) (VAL_AS_##type(a) == VAL_AS_##type(b))

    if (a.type != b.type) { // TODO: how about comparing `int` with `float`
        return false;
    }

    switch (a.type) {
    case HG_VALUE_INT:
        return equal_as(INT);
    case HG_VALUE_FLOAT:
        return equal_as(FLOAT);
    case HG_VALUE_BOOL:
        return equal_as(BOOL);
    case HG_VALUE_NIL:
        return true;
    case HG_VALUE_OBJECT: {
        struct hg_object* obj_a = VAL_AS_OBJ(a);
        struct hg_object* obj_b = VAL_AS_OBJ(a);
        return obj_a->funcs->equal(obj_a, obj_b);
    }
    default:
        unimplemented_("type :%x\n", a.type);
    }
#undef equal_as
*/
}

uint32_t hg_value_hash(struct hg_value a) {
    switch (a.type) {
    case HG_VALUE_INT:
    case HG_VALUE_FLOAT:
        return VAL_AS_INT(a) % UINT32_MAX;
    case HG_VALUE_BOOL:
        return VAL_AS_BOOL(a) ? 1u : 0u;
    case HG_VALUE_NIL:
        return 2u;
    case HG_VALUE_OBJECT: {
        struct hg_object* obj = VAL_AS_OBJ(a);
        if (obj->hash == 0u)
            obj->hash = obj->funcs->hash(obj);
        return obj->hash;
    }
    default:
        unimplemented_("type :%x\n", a.type);
    }
}
//< hg_value

//> hg_value_array
void value_array_init(struct value_array* arr) {
    arr->len      = 0;
    arr->capacity = VALUE_ARRAY_INIT_CAPACITY;
    arr->values   = array_alloc_(struct hg_value, VALUE_ARRAY_INIT_CAPACITY);
}

void value_array_resize(struct value_array* arr) {
    size_t new_capacity = array_new_size_(arr->capacity, arr->len);
    arr->values   = array_realloc_(arr->values, struct hg_value, arr->capacity,
                                 new_capacity);
    arr->capacity = new_capacity;
}

void value_array_free(struct value_array* arr) {
    array_free_(arr->values, struct hg_value, arr->capacity);

    arr->values   = NULL;
    arr->len      = 0;
    arr->capacity = 0;
}
//< hg_value_array
