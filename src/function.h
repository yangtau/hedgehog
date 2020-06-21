#ifndef _HG_FUNCTION_H_
#define _HG_FUNCTION_H_
#include "chunk.h"
#include "object.h"

struct hg_function {
    struct hg_object obj;
    struct chunk chk;
    int argc; // argument count
};

void hg_function_init(struct hg_function* func, int argc);

#endif // _HG_FUNCTION_H_
