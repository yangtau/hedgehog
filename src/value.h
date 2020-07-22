#ifndef _HG_VALUE_H_
#define _HG_VALUE_H_
#include "common.h"
#include "object.h"
#include <string.h>

//> hg_value
enum hg_value_type {
    HG_VALUE_UNDEF = 0, // used only for the language implementation
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

//> VAL_IS_TYPE
/* VAL_IS_TYPE(val): val.type == TYPE
 */
#define VAL_IS_UNDEF(val) ((val).type == HG_VALUE_UNDEF)
#define VAL_IS_NIL(val)   ((val).type == HG_VALUE_NIL)
#define VAL_IS_INT(val)   ((val).type == HG_VALUE_INT)
#define VAL_IS_FLOAT(val) ((val).type == HG_VALUE_FLOAT)
#define VAL_IS_BOOL(val)  ((val).type == HG_VALUE_BOOL)
#define VAL_IS_OBJ(val)   ((val).type == HG_VALUE_OBJECT)
//< VAL_IS_TYPE

//> VAL_AS_TYPE
/* VAL_AS_TYPE(val): hg_value_to_type(val)
 */
#define VAL_AS_INT(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_INT(_value));       \
        _value.as._int;                   \
    })
#define VAL_AS_FLOAT(value)               \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_FLOAT(_value));     \
        _value.as._float;                 \
    })
#define VAL_AS_BOOL(value)                \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_BOOL(_value));      \
        _value.as._bool;                  \
    })
#define VAL_AS_OBJ(value)                 \
    ({                                    \
        struct hg_value _value = (value); \
        assert(VAL_IS_OBJ(_value));       \
        _value.as._obj;                   \
    })
//< VAL_AS_TYPE

//> VAL_TYPE
/* VAL_TYPE(v): type_to_hg_value(v)
 */
#define VAL_BOOL(val)  ((struct hg_value){HG_VALUE_BOOL, {._bool = (val)}})
#define VAL_NIL()      ((struct hg_value){HG_VALUE_NIL, {._int = 0}})
#define VAL_UNDEF()    ((struct hg_value){HG_VALUE_UNDEF, {._int = 0}})
#define VAL_INT(val)   ((struct hg_value){HG_VALUE_INT, {._int = (val)}})
#define VAL_FLOAT(val) ((struct hg_value){HG_VALUE_FLOAT, {._float = (val)}})
#define VAL_OBJ(obj) \
    ((struct hg_value){HG_VALUE_OBJECT, {._obj = (struct hg_object*)(obj)}})

void hg_value_write(struct hg_value a, FILE* fp, bool debug_mode);
bool hg_value_equal(struct hg_value a, struct hg_value b);
uint32_t hg_value_hash(struct hg_value a);
/* hg_value_free:
 * free when `value` is hg_object
 */
static _force_inline_ void hg_value_free(struct hg_value value) {
    if (value.type == HG_VALUE_OBJECT) {
        hg_obj_free_(VAL_AS_OBJ(value));
    }
}
//< hg_value

//> value_array
#define VALUE_ARRAY_INIT_CAPACITY (2u)
struct value_array {
    size_t len;
    size_t capacity;
    struct hg_value* values;
};

void value_array_init(struct value_array* arr);
void value_array_init_with_values(struct value_array* arr, int len,
                                  const struct hg_value* values);
void value_array_free(struct value_array* arr);
size_t value_array_push(struct value_array* arr, struct hg_value val);
struct hg_value value_array_pop(struct value_array* arr);
struct hg_value value_array_get(struct value_array* arr, size_t index);
// change a value already in the array, index < arr->len
void value_array_set(struct value_array* arr, struct hg_value val,
                     size_t index);
//< value_array
#endif // _HG_VALUE_H_
