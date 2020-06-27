#ifndef _HG_FUNCTION_H_
#define _HG_FUNCTION_H_

#include "value.h"
#include "common.h"

//> hg_function
typedef struct hg_value (*builtin_func)(int argc, struct hg_value*);
struct hg_function {
    bool is_builtin;
    int argc; // argument count
    union {
        uint8_t* user_def; // function entry address
        builtin_func builtin;
    } as;
    struct hg_value name;
};

const struct hg_function* hg_function_get_builtins(int* count);

//< hg_function

#endif // _HG_FUNCTION_H_
