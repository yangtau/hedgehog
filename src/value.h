#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include "common.h"
#include <string.h>

enum hg_value_type {
    HG_VALUE_INT,
    HG_VALUE_FLOAT,
    HG_VALUE_BOOL,
    HG_VALUE_NIL,
    // str:
    HG_VALUE_STRING,
    HG_VALUE_ID,
    // ptr:
    HG_VALUE_LIST,
    HG_VALUE_TUPLE,
    HG_VALUE_DICT,
    HG_VALUE_OBJECT,
};

struct hg_value {
    enum hg_value_type type;
    union {
        bool _bool;
        int64_t _int;
        double _float;
        const char* _str;
        void* _ptr;
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
#define VAL_IS_STR(val)   ((val).type == HG_VALUE_STRING)
#define VAL_IS_ID(val)    ((val).type == HG_VALUE_ID)
#define VAL_IS_BOOL(val)  ((val).type == HG_VALUE_BOOL)
#define VAL_IS_PTR(val)                                      \
    ({                                                       \
        enum hg_value_type type = (val).type;                \
        type == HG_VALUE_LIST || type == HG_VALUE_OBJECT ||  \
            type == HG_VALUE_TUPLE || type == HG_VALUE_DICT; \
    })

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
#define VAL_AS_STR(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_STR(_value));       \
        (_value).as._str;                 \
    })
#define VAL_AS_ID(value)                  \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_ID(_value));        \
        (_value).as._str;                 \
    })
#define VAL_AS_BOOL(value)                \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_BOOL(_value));      \
        (_value).as._bool;                \
    })
#define VAL_AS_PTR(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_PTR(_value));       \
        (_value).as._ptr;                 \
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
#define VAL_LIST(val)                                     \
    ({ /*const char* _val = (val);*/                      \
       ((struct hg_value){HG_VALUE_LIST, {._ptr = val}}); \
    })
#define VAL_TUPLE(val)                                     \
    ({ /*const char* _val = (val);*/                       \
       ((struct hg_value){HG_VALUE_TUPLE, {._ptr = val}}); \
    })

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
