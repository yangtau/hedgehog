#ifndef _HG_VM_H_
#define _HG_VM_H_
#include "chunk.h"
#include "common.h"
#include "value.h"

#define STACK_SIZE (1 << 12)
#define FRAME_SIZE (1 << 10)

struct frame {
    uint8_t* rt_addr;
    struct hg_value* slot;
};

struct vm {
    struct chunk* chk;
    uint8_t* ip;
    struct frame* frame_top;
    struct hg_value* stack_top;
    struct hg_value stack[STACK_SIZE];
    struct frame frames[FRAME_SIZE];
};

enum vm_exe_result {
    VM_EXE_OK,
    VM_EXE_RUNTIME_ERROR,
    VM_EXE_IP_OUT_OF_RANGE,
};

void vm_init(struct vm* vm, struct chunk* chk);
void vm_free(struct vm* vm);
enum vm_exe_result vm_run(struct vm* vm);

#endif // _HG_VM_H_
