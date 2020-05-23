#include "value.h"
#include <stdlib.h>

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
