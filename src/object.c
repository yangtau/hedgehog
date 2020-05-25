#include <string.h>
#include "object.h"
#include "memory.h"

//> hg_string
static void hg_string_free(struct hg_object*);
static bool hg_string_equal(struct hg_object*, struct hg_object*);
static uint32_t hg_string_hash(struct hg_object*);
static int hg_string_write(struct hg_object*, FILE*);
static struct hg_object_funcs hg_string_funcs = {.free  = hg_string_free,
                                                 .equal = hg_string_equal,
                                                 .hash  = hg_string_hash,
                                                 .write = hg_string_write};
struct hg_string* hg_string_copy(const char* s, size_t len) {
    struct hg_string* str = flexible_alloc_(struct hg_string, char, len + 1);

    str->obj = (struct hg_object){
        .funcs = &hg_string_funcs, .type = HG_OBJ_STRING, .hash = 0u};

    strncpy(str->str, s, len);
    str->str[len] = '\0';
    str->len      = len;

    return str;
}
struct hg_string* hg_id_copy(const char* s, size_t len) {
    struct hg_string* str = hg_string_copy(s, len);
    str->obj.type         = HG_OBJ_ID;
    return str;
}
static void hg_string_free(struct hg_object* _string) {
    struct hg_string* string = (struct hg_string*)_string;
    flexible_free_(string, struct hg_string, char, string->len + 1);
}
static int hg_string_write(struct hg_object* _string, FILE* fp) {
    struct hg_string* string = (struct hg_string*)_string;
    return fprintf(fp, "%s", string->str);
}
static bool hg_string_equal(struct hg_object* _a, struct hg_object* _b) {
    // all strings are stored in hash table, so if `_a` equals `_b`
    // their pointers should be equivalent.
    return _a == _b;
}
static uint32_t hg_string_hash(struct hg_object* _string) {
    return 0u;
}
//< hg_string
