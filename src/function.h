#ifndef _HG_FUNCTION_H_
#define _HG_FUNCTION_H_
#include "chunk.h"
#include "object.h"

struct hg_function {
    struct hg_object obj;
    int argc;      // argument count
    uint8_t* addr; // function entry address
};

void hg_function_init(struct hg_function* func, int argc, uint8_t* addr);

#endif // _HG_FUNCTION_H_
