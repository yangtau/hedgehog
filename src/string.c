#include <string.h>
#include "string.h"
#include "memory.h"
#include "value.h"

//> hg_string_pool
//< hg_string_pool

//> hg_string
static void hg_string_free(struct hg_object*);
static bool hg_string_equal(struct hg_object*, struct hg_object*);
static uint32_t hg_string_hash(struct hg_object*);
static int hg_string_write(struct hg_object*, FILE*);
static struct hg_object_funcs hg_string_funcs = {
    .free  = hg_string_free,
    .equal = hg_string_equal,
    .hash  = hg_string_hash,
    .write = hg_string_write,
};

struct hg_string* hg_string_copy(const char* s, size_t len) {
    struct hg_string* str = flexible_alloc_(struct hg_string, char, len + 1);

    str->obj = (struct hg_object){
        .funcs = &hg_string_funcs,
        .type  = HG_OBJ_STRING,
        .hash  = 0u,
    };

    strncpy(str->str, s, len);
    str->str[len] = '\0';
    str->len      = len;

    return str;
}

static void hg_string_free(struct hg_object* _string) {
    struct hg_string* string = (struct hg_string*)_string;
    flexible_free_(string, struct hg_string, char, string->len + 1);
}

static int hg_string_write(struct hg_object* _string, FILE* fp) {
    struct hg_string* string = (struct hg_string*)_string;
    return fprintf(fp, "\"%.*s\"", (int)string->len, string->str);
}

static bool hg_string_equal(struct hg_object* _a, struct hg_object* _b) {
    assert(_a->type == HG_OBJ_STRING && _b->type == HG_OBJ_STRING);

    struct hg_string* str_a = (struct hg_string*)_a;
    struct hg_string* str_b = (struct hg_string*)_b;
    if (str_a->len != str_b->len)
        return false;
    if (_a->funcs->hash(_a) != _b->funcs->hash(_b))
        return false;
    return strncmp(str_a->str, str_b->str, str_a->len) == 0;
}

static uint32_t hg_string_hash(struct hg_object* _string) {
    if (_string->hash != 0u)
        return _string->hash;

    struct hg_string* string = (struct hg_string*)_string;
    uint32_t hash            = 2166136261u;

    for (size_t i = 0; i < string->len; i++) {
        hash ^= string->str[i];
        hash *= 16777619;
    }
    hash += 1u;
    if (hash == 0u)
        hash += 1u;

    return hash;
}
//< hg_string

//> hg_symbol
static bool hg_symbol_equal(struct hg_object* _a, struct hg_object* _b) {
    struct hg_string* str_a = (struct hg_string*)_a;
    struct hg_string* str_b = (struct hg_string*)_b;
    if (str_a->len != str_b->len)
        return false;
    if (_a->funcs->hash(_a) != _b->funcs->hash(_b))
        return false;
    return strncmp(str_a->str, str_b->str, str_a->len) == 0;

    // TODO: symbol
    // return _a == _b;
}

static int hg_symbol_write(struct hg_object* _string, FILE* fp) {
    struct hg_string* string = (struct hg_string*)_string;
    return fprintf(fp, "%.*s", (int)string->len, string->str);
}

static struct hg_object_funcs hg_symbol_funcs = {
    .free  = hg_string_free,
    .equal = hg_symbol_equal,
    .hash  = hg_string_hash,
    .write = hg_symbol_write,
};

struct hg_string* hg_symbol_copy(const char* s, size_t len) {
    struct hg_string* str = flexible_alloc_(struct hg_string, char, len + 1);

    str->obj = (struct hg_object){
        .funcs = &hg_symbol_funcs,
        .type  = HG_OBJ_SYMBOL,
        .hash  = 0u,
    };

    strncpy(str->str, s, len);
    str->str[len] = '\0';
    str->len      = len;

    return str;
}
//< hg_symbol
