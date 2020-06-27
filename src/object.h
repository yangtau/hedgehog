#ifndef _HG_OBJECT_H_
#define _HG_OBJECT_H_
#include "common.h"

//> hg_object
enum hg_object_type {
    HG_OBJ_STRING,
    HG_OBJ_SYMBOL,
    HG_OBJ_MAP,
    HG_OBJ_LIST,
    HG_OBJ_TUPLE,
    HG_OBJ_FUNCTION,
};

struct hg_object_funcs;

struct hg_object {
    enum hg_object_type type;
    struct hg_object_funcs* funcs;
    uint32_t hash; // 0 indicates that the hash has not been computed yet
    // real range of hash is [1, UINT32_T_MAX]
};

struct hg_object_funcs {
    void (*free)(struct hg_object*);
    uint32_t (*hash)(struct hg_object*);
    int (*write)(struct hg_object*, FILE*);
    bool (*equal)(struct hg_object*, struct hg_object*);
    struct hg_object* (*add)(struct hg_object*, struct hg_object*);
};
#define hg_obj_write_(obj, fp)                             \
    ({                                                     \
        struct hg_object* _obj = (struct hg_object*)(obj); \
        _obj->funcs->write(_obj, (fp));                    \
    })
#define hg_obj_hash_(obj)                                  \
    ({                                                     \
        struct hg_object* _obj = (struct hg_object*)(obj); \
        _obj->funcs->hash(_obj);                           \
    })
#define hg_obj_free_(obj)                                  \
    ({                                                     \
        struct hg_object* _obj = (struct hg_object*)(obj); \
        _obj->funcs->free(_obj);                           \
    })
#define hg_obj_equal_(obj1, obj2)                            \
    ({                                                       \
        struct hg_object* _obj1 = (struct hg_object*)(obj1); \
        struct hg_object* _obj2 = (struct hg_object*)(obj2); \
        _obj1->funcs->equal(_obj1, _obj2);                   \
    })
#define hg_obj_add_(obj1, obj2)                            \
    ({                                                       \
        struct hg_object* _obj1 = (struct hg_object*)(obj1); \
        struct hg_object* _obj2 = (struct hg_object*)(obj2); \
        _obj1->funcs->add(_obj1, _obj2);                   \
    })

//< hg_object
#endif // _HG_OBJECT_H_
