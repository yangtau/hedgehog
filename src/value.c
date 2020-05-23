#include <stdlib.h>
#include "value.h"

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
    case HG_VALUE_STRING:
        fprintf(fp, "%s", VAL_AS_STR(a));
        break;
    case HG_VALUE_ID:
        fprintf(fp, "%s", VAL_AS_ID(a));
        break;
    case HG_VALUE_LIST:
        break;
    case HG_VALUE_TUPLE:
        break;
    case HG_VALUE_DICT:
        break;
    case HG_VALUE_OBJECT:
        break;
    default:
        unimplemented("type :%x", a.type);
    }
}

bool hg_value_equal(struct hg_value a, struct hg_value b) {
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
    case HG_VALUE_ID: {
        const char* t = VAL_AS_ID(b);
        const char* s = VAL_AS_ID(a);
        return strcmp(t, s) == 0;
    }
    case HG_VALUE_STRING: {
        const char* t = VAL_AS_STR(b);
        const char* s = VAL_AS_STR(a);
        return strcmp(t, s) == 0;
    }
    default:
        unimplemented("type :%x", a.type);
    }
    return false;
#undef equal_as
}

struct hg_value hg_value_str_len_new(const char* s, size_t len) {
    struct hg_value val;
    val.type = HG_VALUE_STRING;

    char* t = malloc(sizeof(char) * (len + 1));
    stpncpy(t, s, len);
    t[len] = '\0';

    val.as._str = t;
    return val;
}

struct hg_value hg_value_id_new(const char* id) {
    struct hg_value val = hg_value_str_len_new(id, strlen(id));
    val.type            = HG_VALUE_ID;
    return val;
}
