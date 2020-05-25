#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include "common.h"
#include "object.h"
#include <string.h>

enum hg_value_type {
    HG_VALUE_INT,
    HG_VALUE_FLOAT,
    HG_VALUE_BOOL,
    HG_VALUE_NIL,
    HG_VALUE_OBJECT,
};

struct hg_value {
    enum hg_value_type type;
    union {
        bool _bool;
        int64_t _int;
        double _float;
        struct hg_object* _obj;
    } as;
};

/* hg_value_free:
 * free when `value` is hg_object
 */
void hg_value_free(struct hg_value value);

void hg_value_write(struct hg_value a, FILE* fp);
bool hg_value_equal(struct hg_value a, struct hg_value b);

/* VAL_IS_TYPE(val): val.type == TYPE
 */
#define VAL_IS_NIL(val)   ((val).type == HG_VALUE_NIL)
#define VAL_IS_INT(val)   ((val).type == HG_VALUE_INT)
#define VAL_IS_FLOAT(val) ((val).type == HG_VALUE_FLOAT)
#define VAL_IS_BOOL(val)  ((val).type == HG_VALUE_BOOL)
#define VAL_IS_OBJ(val)   ((val).type == HG_VALUE_OBJECT)

/* VAL_AS_TYPE(val): hg_value_to_type(val)
 */
#define VAL_AS_INT(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_INT(_value));       \
        (_value).as._int;                 \
    })
#define VAL_AS_FLOAT(value)               \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_FLOAT(_value));     \
        (_value).as._float;               \
    })
#define VAL_AS_BOOL(value)                \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_BOOL(_value));      \
        (_value).as._bool;                \
    })
#define VAL_AS_OBJ(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_OBJ(_value));       \
        (_value).as._obj;                 \
    })

/* VAL_TYPE(v): type_to_hg_value(v)
 */
#define VAL_BOOL(val)  ((struct hg_value){HG_VALUE_BOOL, {._bool = (val)}})
#define VAL_NIL()      ((struct hg_value){HG_VALUE_NIL, {._int = 0}})
#define VAL_INT(val)   ((struct hg_value){HG_VALUE_INT, {._int = (val)}})
#define VAL_FLOAT(val) ((struct hg_value){HG_VALUE_FLOAT, {._float = (val)}})
#define VAL_STR_LEN(val, len)           \
    ((struct hg_value){HG_VALUE_OBJECT, \
                       {._obj = (struct hg_object*)hg_string_copy(val, len)}})
#define VAL_STR(val)                 \
    ({                               \
        const char* _s = (val);      \
        VAL_STR_LEN(_s, strlen(_s)); \
    })
#define VAL_ID(val)                                                  \
    ({                                                               \
        const char* _s = (val);                                      \
        (struct hg_value){                                           \
            HG_VALUE_OBJECT,                                         \
            {._obj = (struct hg_object*)hg_id_copy(_s, strlen(_s))}}; \
    })


// value_array:
#define INITIAL_VALUE_ARRAY_LEN 4
struct value_array {
    size_t len;
    size_t capacity;
    struct hg_value values[];
};

struct value_array* value_array_new();
struct value_array* value_array_resize(struct value_array* arr, size_t new_len);
void valu_array_free(struct value_array* arr);
#endif // _HG_VALUE_H_
