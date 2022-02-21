#include "vm.h"
#include "memory.h"

struct hg_value hg_vm_execute(struct hg_vm_state* vm,
                              struct hg_function* func) {
#define _pc    (vm->pc)
#define _frame (vm->frame_cur)
#define _sp    (vm->frame_cur->base)
    _pc              = func->code;
    _sp              = vm->stack;
    _frame->ret_addr = NULL;
    _frame->func     = func;

    while (true) {
        enum hg_opcode op = _pc->op;

        // arithmetic:
        int32_t rt = _pc->as.ari.rt;
        int32_t ra = _pc->as.ari.ra;
        int32_t rb = _pc->as.ari.rb;
        switch (op) {
        case HG_OP_ADD: {
            _sp[rt].as.i = _sp[ra].as.i + _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_SUB: {
            _sp[rt].as.i = _sp[ra].as.i - _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_MUL: {
            _sp[rt].as.i = _sp[ra].as.i * _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_DIV: {
            _sp[rt].as.i = _sp[ra].as.i / _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_MOD: {
            _sp[rt].as.i = _sp[ra].as.i % _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_AND: {
            _sp[rt].as.i = _sp[ra].as.i && _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_OR: {
            _sp[rt].as.i = _sp[ra].as.i || _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_GT: {
            _sp[rt].as.i = _sp[ra].as.i > _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_LT: {
            _sp[rt].as.i = _sp[ra].as.i < _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_EQ: {
            _sp[rt].as.i = _sp[ra].as.i == _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_NEQ: {
            _sp[rt].as.i = _sp[ra].as.i != _sp[rb].as.i;
            _pc++;
        } break;
        case HG_OP_NOT: {
            _sp[rt].as.i = !(_sp[ra].as.i);
            _pc++;
        } break;
        case HG_OP_JUMPFALSE: {
            int32_t rx    = _pc->as.jump.rx;
            int32_t label = _pc->as.jump.label;
            if (_sp[rx].as.i) {
                _pc += label;
            } else {
                _pc++;
            }
        } break;
        case HG_OP_JUMP: {
            int32_t label = _pc->as.jump.label;
            _pc += label;
        } break;
        case HG_OP_CALL: {
            int32_t rx = _pc->as.call.rx;
            int32_t n  = _pc->as.call.n;
            int32_t m  = _pc->as.call.m;

            struct hg_function* fn = _sp[rx].as.func;
            assert((size_t)n == fn->num_args);
            assert((size_t)m == fn->num_returns);

            //  push frame
            struct hg_call_frame* new_frame =
                _frame + 1; // TODO: check frame size

            new_frame->func     = fn;
            new_frame->ret_addr = _pc + 1;
            new_frame->base     = _sp + rx; // TODO: check stack size

            _frame = new_frame;

            _pc = fn->code;
        } break;
        case HG_OP_RETURN: {
            _pc = _frame->ret_addr;
            _frame--;
            if (_frame->ret_addr == NULL) {
                // entry function
                return _sp[0];
            }

        } break;
        case HG_OP_MOVE: {
            int32_t rx = _pc->as.move.rx;
            int32_t rt = _pc->as.move.rt;
            _sp[rt]    = _sp[rx];
            _pc++;
        } break;
        case HG_OP_LOADC: {
            int32_t rx = _pc->as.load.rx;
            int32_t n  = _pc->as.load.n;
            _sp[rx]    = _frame->func->constants[n];
            _pc++;
        } break;
        default:
            unreachable_();
        }
    }
    return (struct hg_value){.as.i = 0};
#undef _pc
}
