#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include "common.h"
#include <string.h>

enum hg_value_type {
    HG_VALUE_INT,
    HG_VALUE_FLOAT,
    HG_VALUE_BOOL,
    HG_VALUE_NIL,
    HG_VALUE_OBJECT,
};

struct hg_object_funcs {
    void (*free)(void*);
    void* (*new)(void*);
};

struct hg_object {
    struct hg_object_funcs* funcs;
    void* ptr;
};

struct hg_value {
    enum hg_value_type type;
    union {
        bool _bool;
        int64_t _int;
        double _float;
        const struct hg_object* _obj;
    } as;
};

void hg_value_write(struct hg_value a, FILE* fp);
bool hg_value_equal(struct hg_value a, struct hg_value b);

struct hg_value hg_value_str_len_new(const char* s, size_t len);
struct hg_value hg_value_id_new(const char* id);

/* VAL_IS_TYPE(val): val.type == TYPE
 */
#define VAL_IS_NIL(val)   ((val).type == HG_VALUE_NIL)
#define VAL_IS_INT(val)   ((val).type == HG_VALUE_INT)
#define VAL_IS_FLOAT(val) ((val).type == HG_VALUE_FLOAT)
#define VAL_IS_BOOL(val)  ((val).type == HG_VALUE_BOOL)
#define VAL_IS_OBJ(val)   ((val).type == HG_VALUE_OBJECT)

/* VAL_AS_TYPE(val): hg_value_to_type(val)
 */
#define VAL_AS_INT(value)          \
    ({                             \
        assert(VAL_IS_INT(value)); \
        (value).as._int;           \
    })
#define VAL_AS_FLOAT(value)          \
    ({                               \
        assert(VAL_IS_FLOAT(value)); \
        (value).as._float;           \
    })
#define VAL_AS_BOOL(value)          \
    ({                              \
        assert(VAL_IS_BOOL(value)); \
        (value).as._bool;           \
    })
#define VAL_AS_OBJ(value)          \
    ({                             \
        assert(VAL_IS_OBJ(value)); \
        (value).as._obj;           \
    })

/* VAL_TYPE(v): type_to_hg_value(v)
 */
#define VAL_BOOL(val)         ((struct hg_value){HG_VALUE_BOOL, {._bool = (val)}})
#define VAL_NIL()             ((struct hg_value){HG_VALUE_NIL, {._int = 0}})
#define VAL_INT(val)          ((struct hg_value){HG_VALUE_INT, {._int = (val)}})
#define VAL_FLOAT(val)        ((struct hg_value){HG_VALUE_FLOAT, {._float = (val)}})
#define VAL_STR_LEN(val, len) hg_value_str_len_new(val, len)
#define VAL_STR(val)          hg_value_str_len_new(val, strlen(val))
#define VAL_ID(val)           hg_value_id_new(val)
#define VAL_LIST(val)         hg_value_list_new(val)
#define VAL_TUPLE(val)        hg_value_tuple_new(val)
#endif // _HG_VALUE_H_
