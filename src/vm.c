#include "vm.h"
#include "common.h"

void vm_init(struct vm* vm, struct chunk* chk) {
    vm->chk       = chk;
    vm->ip        = chk->code;
    vm->stack_top = vm->stack;
}

void vm_free(struct vm* vm) {
    vm->chk = NULL;
    vm->ip  = NULL;
}

static inline void push(struct vm* vm, struct hg_value val) {
    if (vm->stack_top >= vm->stack + STACK_SIZE)
        error_("stack overflow");
    *vm->stack_top++ = val;
}

static inline struct hg_value pop(struct vm* vm) {
    if (vm->stack_top <= vm->stack)
        error_("pop empty stack");
    return *(--vm->stack_top);
}

enum vm_exe_result vm_run(struct vm* vm) {
// TODO: check range limit
#define read_byte_() (*vm->ip++)
#define read_word_()                             \
    ({                                           \
        vm->ip += 2;                             \
        (uint16_t) vm->ip[-2] << 8 | vm->ip[-1]; \
    })
#define read_static_() value_array_get(&vm->chk->statics, read_word_())
#define binary_arith_op_(op)                                             \
    do {                                                                 \
        struct hg_value b = pop(vm);                                     \
        struct hg_value a = pop(vm);                                     \
        if (VAL_IS_INT(a) && VAL_IS_INT(b)) {                            \
            push(vm, VAL_INT(VAL_AS_INT(a) op VAL_AS_INT(b)));           \
        } else if (VAL_IS_INT(a) && VAL_IS_FLOAT(b)) {                   \
            push(vm, VAL_FLOAT(VAL_AS_INT(a) op VAL_AS_FLOAT(b)));       \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_INT(b)) {                   \
            push(vm, VAL_FLOAT(VAL_AS_FLOAT(a) op VAL_AS_INT(b)));       \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_FLOAT(b)) {                 \
            push(vm, VAL_FLOAT(VAL_AS_FLOAT(a) op VAL_AS_FLOAT(b)));     \
        } else {                                                         \
            error_("unsupported operand type for " #op ":%x %x", a.type, \
                   b.type);                                              \
        }                                                                \
    } while (0)
#define binary_cmp_op_(op)                                               \
    do {                                                                 \
        struct hg_value b = pop(vm);                                     \
        struct hg_value a = pop(vm);                                     \
        if (VAL_IS_INT(a) && VAL_IS_INT(b)) {                            \
            push(vm, VAL_BOOL(VAL_AS_INT(a) op VAL_AS_INT(b)));          \
        } else if (VAL_IS_INT(a) && VAL_IS_FLOAT(b)) {                   \
            push(vm, VAL_BOOL(VAL_AS_INT(a) op VAL_AS_FLOAT(b)));        \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_INT(b)) {                   \
            push(vm, VAL_BOOL(VAL_AS_FLOAT(a) op VAL_AS_INT(b)));        \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_FLOAT(b)) {                 \
            push(vm, VAL_BOOL(VAL_AS_FLOAT(a) op VAL_AS_FLOAT(b)));      \
        } else {                                                         \
            error_("unsupported operand type for " #op ":%x %x", a.type, \
                   b.type);                                              \
        }                                                                \
    } while (0)

    while (1) {
        uint8_t ins;
        switch (ins = read_byte_()) {
        case OP_NOP:
            break;
        case OP_GET_STATIC:
            push(vm, read_static_());
            break;
        case OP_NIL:
            push(vm, VAL_NIL());
            break;
        case OP_TRUE:
            push(vm, VAL_BOOL(true));
            break;
        case OP_FALSE:
            push(vm, VAL_BOOL(false));
            break;
        case OP_EQUAL:
            push(vm, VAL_BOOL(hg_value_equal(pop(vm), pop(vm))));
            break;
        case OP_GREATER:
            binary_cmp_op_(>);
            break;
        case OP_GREATER_EQUAL:
            binary_cmp_op_(>=);
            break;
        case OP_LESS_EQUAL:
            binary_cmp_op_(<=);
            break;
        case OP_LESS:
            binary_cmp_op_(<);
            break;
        case OP_ADD:
            binary_arith_op_(+);
            break;
        case OP_SUBTRACT:
            binary_arith_op_(-);
            break;
        case OP_MULTIPLY:
            binary_arith_op_(*);
            break;
        case OP_DIVIDE:
            binary_arith_op_(/);
            break;
        case OP_MODULO: {
            struct hg_value a = pop(vm);
            struct hg_value b = pop(vm);
            if (VAL_IS_INT(a) && VAL_IS_INT(b)) {
                push(vm, VAL_INT(VAL_AS_INT(a) % VAL_AS_INT(b)));
            } else {
                error_("unsupported operand type for %%:%x %x", a.type, b.type);
            }
        } break;
        case OP_NOT: {
            struct hg_value a = pop(vm);
            if (VAL_IS_BOOL(a)) {
                push(vm, VAL_BOOL(!VAL_AS_BOOL(a)));
            } else {
                error_("unsupported operand type for !: %x", a.type);
            }
        } break;
        case OP_NEGATE: {
            struct hg_value a = pop(vm);
            if (VAL_IS_INT(a)) {
                push(vm, VAL_INT(-VAL_AS_INT(a)));
            } else if (VAL_IS_FLOAT(a)) {
                push(vm, VAL_FLOAT(-VAL_AS_FLOAT(a)));
            } else {
                error_("unsupported operand type for !: %x", a.type);
            }
        } break;
        case OP_POP:
            pop(vm);
            break;
        case OP_PUSH:
            break;
        case OP_GET_LOCAL:
            break;
        case OP_SET_LOCAL:
            break;
        case OP_JUMP:
            break;
        case OP_JUMP_IF_FALSE:
            break;
        case OP_JUMP_BACK:
            break;
        case OP_CALL:
            break;
        }
        vm->ip++;
    }
    return VM_EXE_OK;
#undef read_byte_
#undef read_word_
#undef read_static_
#undef binary_arith_op_
#undef binary_cmp_op_
}
