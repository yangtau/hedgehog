#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
struct hg_value {
    enum {
        HG_VALUE_INT,
        HG_VALUE_FLOAT,
        HG_VALUE_STRING,
        HG_VALUE_BOOL,
        HG_VALUE_NIL,
        HG_VALUE_LIST,
        HG_VALUE_TUPLE,
        HG_VALUE_DICT,
        HG_VALUE_OBJECT,
        HG_VALUE_ID,
    } type;
    union {
        bool _bool;
        int64_t _int;
        double _float;
        const char* _str;
        void* _ptr;
    } as;
};

struct hg_value hg_value_str_len_new(const char* s, size_t len);
struct hg_value hg_value_id_new(const char* id);

#define VAL_AS_INT(value)   ((value).as._int)
#define VAL_AS_FLOAT(value) ((value).as._float)
#define VAL_AS_STR(value)   ((value).as._str)
#define VAL_AS_ID(value)    ((value).as._str)
#define VAL_AS_BOOL(value)  ((value).as._bool)
#define VAL_AS_PTR(value)   ((value).as._ptr)

#define HG_BOOL_VAL(val)         ((struct hg_value){HG_VALUE_BOOL, {._bool = (val)}})
#define HG_NIL_VAL               ((struct hg_value){HG_VALUE_NIL, {._int = 0}})
#define HG_INT_VAL(val)          ((struct hg_value){HG_VALUE_INT, {._int = (val)}})
#define HG_FLOAT_VAL(val)        ((struct hg_value){HG_VALUE_FLOAT, {._float = (val)}})
#define HG_STR_LEN_VAL(val, len) hg_value_str_len_new(val, len)
#define HG_STR_VAL(val)          hg_value_str_len_new(val, strlen(val))
#define HG_ID_VAL(val)           hg_value_id_new(val)
#define HG_LIST_VAL(val)                                          \
    ({ /*const char* _val = (val);*/                              \
       ((struct hg_value){.type = HG_VALUE_LIST, {._ptr = val}}); \
    })
#define HG_TUPLE_VAL(val)                                          \
    ({ /*const char* _val = (val);*/                               \
       ((struct hg_value){.type = HG_VALUE_TUPLE, {._ptr = val}}); \
    })

#endif // _HG_VALUE_H_
