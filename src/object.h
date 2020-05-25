#ifndef _HG_OBJECT_H_
#define _HG_OBJECT_H_
#include "common.h"

//> hg_object
enum hg_object_type {
    HG_OBJ_STRING,
    HG_OBJ_ID,
    HG_OBJ_MAP,
    HG_OBJ_LIST,
    HG_OBJ_TUPLE
};

struct hg_object_funcs;

struct hg_object {
    enum hg_object_type type;
    struct hg_object_funcs* funcs;
    uint32_t hash;
};

struct hg_object_funcs {
    void (*free)(struct hg_object*);
    uint32_t (*hash)(struct hg_object*);
    int (*write)(struct hg_object*, FILE*);
    bool (*equal)(struct hg_object*, struct hg_object*);
};
//< hg_object

//> hg_string
struct hg_string {
    struct hg_object obj;
    size_t len;
    char str[];
};
// hg_string_copy: copy `s` to create a new hg_string
struct hg_string* hg_string_copy(const char* s, size_t len);
struct hg_string* hg_id_copy(const char* s, size_t len);
//< hg_string
#endif
