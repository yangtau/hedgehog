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

static inline void print_stack_info(struct vm* vm) {
    printf("### stack top    ###>\n");
    int i = 0;
    for (struct hg_value* p = vm->stack_top - 1; p >= vm->stack; p--) {
        printf("0x%x ", i);
        hg_value_write(*p, stdout);
        printf("\n");
    }
    printf("### stack bottom ###<\n");
}

enum vm_exe_result vm_run(struct vm* vm) {
// TODO: check range limit
#define consume_byte_() (*vm->ip++)
#define read_word_()    (uint16_t) vm->ip[0] << 8 | vm->ip[1]
#define consume_word_()                          \
    ({                                           \
        vm->ip += 2;                             \
        (uint16_t) vm->ip[-2] << 8 | vm->ip[-1]; \
    })
#define read_static_() value_array_get(&vm->chk->statics, consume_word_())
#define set_static_(val) \
    value_array_set(&vm->chk->statics, val, consume_word_())
#define read_const_() value_array_get(&vm->chk->consts, consume_word_())
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
        case OP_GET_CONST:
            push(vm, read_const_());
            break;
        case OP_GET_STATIC:
            push(vm, read_static_());
            break;
        case OP_SET_STATIC: {
            struct hg_value t = pop(vm);
            set_static_(t);
        } break;
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
        case OP_GET_LOCAL:
            break;
        case OP_SET_LOCAL:
            break;
        case OP_JUMP: {
            uint16_t off = read_word_();
            vm->ip += off;
        } break;
        case OP_JUMP_IF_FALSE: {
            uint16_t off = read_word_();
            if (!VAL_AS_BOOL(pop(vm))) {
                vm->ip += off;
            } else {
                vm->ip += 2; // consume a word
            }
        } break;
        case OP_JUMP_BACK: {
            uint16_t off = read_word_();
            vm->ip -= off;
        } break;
        case OP_CALL:
            break;
        }
        print_stack_info(vm);
        printf("\n");
    }
    return VM_EXE_OK;
#undef consume_byte_
#undef consume_word_
#undef read_word_
#undef read_static_
#undef set_static_
#undef read_const_
#undef binary_arith_op_
#undef binary_cmp_op_
}
