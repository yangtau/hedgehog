#ifndef _HG_VM_H_
#define _HG_VM_H_
#include "chunk.h"
#include "common.h"
#include "value.h"

#define STACK_SIZE (1<<16)
struct vm {
    struct chunk* chk;
    uint8_t* ip;
    struct hg_value *stack_top;
    struct hg_value stack[STACK_SIZE];
};

enum vm_exe_result {
    VM_EXE_OK,
    VM_EXE_RUNTIME_ERROR,
};

void vm_init(struct vm* vm, struct chunk* chk);
void vm_free(struct vm* vm);
enum vm_exe_result vm_run(struct vm* vm);

#endif // _HG_VM_H_
