#include "vm.h"
#include "common.h"
#include "function.h"

//> vm
void vm_init(struct vm* vm, struct chunk* chk) {
    vm->chk       = chk;
    vm->ip        = chk->code;
    vm->stack_top = vm->stack;
    vm->frame_top = vm->frames;

    // initialize the global frame
    *vm->frame_top = (struct frame){
        .rt_addr = NULL,
        .rt      = VAL_UNDEF(),
        .slot    = vm->stack,
    };
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
    assert(vm->stack_top > vm->stack);
    return *(--vm->stack_top);
}

static inline void call(struct vm* vm, struct hg_function* func) {
}

static inline void print_stack_info(struct vm* vm) {
    printf("### stack top    ###>\n");
    for (struct hg_value* p = vm->stack_top - 1; p >= vm->stack; p--) {
        printf("0x%lx ", p - vm->stack);
        hg_value_write(*p, stdout);
        printf("\n");
    }
    printf("### stack bottom ###<\n");
}

enum vm_exe_result vm_run(struct vm* vm) {
// TODO: check the range of ip when debug
#define consume_byte_()                                \
    ({                                                 \
        assert(vm->ip < vm->chk->code + vm->chk->len); \
        *vm->ip++;                                     \
    })
#define read_word_()                                       \
    ({                                                     \
        assert(vm->ip + 1 < vm->chk->code + vm->chk->len); \
        (uint16_t) vm->ip[0] << 8 | vm->ip[1];             \
    })
#define consume_word_()                                    \
    ({                                                     \
        assert(vm->ip + 1 < vm->chk->code + vm->chk->len); \
        vm->ip += 2;                                       \
        (uint16_t) vm->ip[-2] << 8 | vm->ip[-1];           \
    })
#define binary_arith_op_(op)                                         \
    do {                                                             \
        struct hg_value b = pop(vm);                                 \
        struct hg_value a = pop(vm);                                 \
        if (VAL_IS_INT(a) && VAL_IS_INT(b)) {                        \
            push(vm, VAL_INT(VAL_AS_INT(a) op VAL_AS_INT(b)));       \
        } else if (VAL_IS_INT(a) && VAL_IS_FLOAT(b)) {               \
            push(vm, VAL_FLOAT(VAL_AS_INT(a) op VAL_AS_FLOAT(b)));   \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_INT(b)) {               \
            push(vm, VAL_FLOAT(VAL_AS_FLOAT(a) op VAL_AS_INT(b)));   \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_FLOAT(b)) {             \
            push(vm, VAL_FLOAT(VAL_AS_FLOAT(a) op VAL_AS_FLOAT(b))); \
        } else {                                                     \
            error_("unsupported operand type for " #op               \
                   ": value_type: %x value_type: %x",                \
                   a.type, b.type);                                  \
        }                                                            \
    } while (0)
#define binary_cmp_op_(op)                                          \
    do {                                                            \
        struct hg_value b = pop(vm);                                \
        struct hg_value a = pop(vm);                                \
        if (VAL_IS_INT(a) && VAL_IS_INT(b)) {                       \
            push(vm, VAL_BOOL(VAL_AS_INT(a) op VAL_AS_INT(b)));     \
        } else if (VAL_IS_INT(a) && VAL_IS_FLOAT(b)) {              \
            push(vm, VAL_BOOL(VAL_AS_INT(a) op VAL_AS_FLOAT(b)));   \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_INT(b)) {              \
            push(vm, VAL_BOOL(VAL_AS_FLOAT(a) op VAL_AS_INT(b)));   \
        } else if (VAL_IS_FLOAT(a) && VAL_IS_FLOAT(b)) {            \
            push(vm, VAL_BOOL(VAL_AS_FLOAT(a) op VAL_AS_FLOAT(b))); \
        } else {                                                    \
            error_("unsupported operand type for " #op              \
                   ": value_type: %x value_type: %x",               \
                   a.type, b.type);                                 \
        }                                                           \
    } while (0)

    while (vm->ip < vm->chk->code + vm->chk->len) {
        chunk_disassemble_ins(vm->chk, vm->ip - vm->chk->code);
        uint8_t ins;

        switch (ins = consume_byte_()) {
        case OP_NOP:
            break;

        case OP_QUIT:
            return VM_EXE_OK;

        case OP_GET_CONST:
            push(vm, value_array_get(&vm->chk->consts, consume_word_()));
            break;

        case OP_GET_STATIC:
            push(vm, value_array_get(&vm->chk->statics, consume_word_()));
            break;

        case OP_SET_STATIC:
            value_array_set(&vm->chk->statics, pop(vm), consume_word_());
            break;

        case OP_GET_LOCAL:
            push(vm, vm->frame_top->slot[consume_word_()]);
            break;

        case OP_SET_LOCAL:
            vm->frame_top->slot[consume_word_()] = pop(vm);
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

        case OP_JUMP:
            vm->ip += read_word_();
            break;

        case OP_JUMP_IF_FALSE:
            // consume a word or jump to the target
            vm->ip += VAL_AS_BOOL(pop(vm)) ? 2 : read_word_();
            break;

        case OP_JUMP_BACK:
            vm->ip -= read_word_();
            break;

        case OP_CALL:
            break;
        }
        print_stack_info(vm);
        printf("\n");
    }
    return VM_EXE_IP_OUT_OF_RANGE;
#undef consume_byte_
#undef consume_word_
#undef binary_arith_op_
#undef binary_cmp_op_
}
//> vm
