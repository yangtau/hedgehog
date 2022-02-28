#include "vm.h"
#include "memory.h"

struct hg_value hg_vm_execute(struct hg_vm_state* vm, struct hg_function* func) {
    hg_stk_ptr sp = vm->stack;

    struct hg_call_frame* cur_frame = vm->frames;
    // initialize entry frame
    cur_frame->func     = func;
    cur_frame->base     = sp;
    cur_frame->ret_addr = NULL;

    struct hg_ins* pc = cur_frame->func->code;

#define vdispatch(code) switch (code->op)
#define vcase(x, y) \
    case x:         \
        y;          \
        break;
#define vdefault(y) \
    default:        \
        y

#ifdef HG_VM_JUMPTABLE
#include "vm_jumptable.h"
#endif // HG_VM_JUMPTABLE

    while (true) {
        vdispatch(pc) {
            vcase(HG_OP_ADD, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i + sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_SUB, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i - sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_MUL, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i * sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_DIV, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i / sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_MOD, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i % sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_AND, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i && sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_OR, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i || sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_GT, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i > sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_LT, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i < sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_EQ, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i == sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_NEQ, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;
                int32_t rb = pc->as.ari.rb;

                sp[rt].as.i = sp[ra].as.i != sp[rb].as.i;
                pc++;
            });
            vcase(HG_OP_NOT, {
                int32_t rt = pc->as.ari.rt;
                int32_t ra = pc->as.ari.ra;

                sp[rt].as.i = !(sp[ra].as.i);
                pc++;
            });
            vcase(HG_OP_JUMPFALSE, {
                int32_t rx    = pc->as.jump.rx;
                int32_t label = pc->as.jump.label;
                if (sp[rx].as.i) {
                    pc += label;
                } else {
                    pc++;
                }
            });
            vcase(HG_OP_JUMP, {
                int32_t label = pc->as.jump.label;
                pc += label;
            });
            vcase(HG_OP_CALL, {
                int32_t rx = pc->as.call.rx;
                int32_t n  = pc->as.call.n;
                int32_t m  = pc->as.call.m;

                struct hg_function* fn = sp[rx].as.func;
                assert((size_t)n == fn->num_args);
                assert((size_t)m == fn->num_returns);

                //  push frame; TODO: check frame size
                struct hg_call_frame* new_frame = cur_frame + 1;

                new_frame->func     = fn;
                new_frame->ret_addr = pc + 1;
                new_frame->base     = sp + rx; // TODO: check stack size

                cur_frame = new_frame;

                sp = new_frame->base;
                pc = fn->code;
            });
            vcase(HG_OP_RETURN, {
                pc = cur_frame->ret_addr;
                cur_frame--;
                if (cur_frame->ret_addr == NULL) {
                    // entry function
                    return sp[0];
                }
            });
            vcase(HG_OP_MOVE, {
                int32_t rx = pc->as.move.rx;
                int32_t rt = pc->as.move.rt;
                sp[rt]     = sp[rx];
                pc++;
            });
            vcase(HG_OP_LOADC, {
                int32_t rx = pc->as.load.rx;
                int32_t n  = pc->as.load.n;
                sp[rx]     = cur_frame->func->constants[n];
                pc++;
            });
            vdefault({ unreachable_(); });
        }
    }
    return (struct hg_value){.as.i = 0};
#undef vdispatch
#undef vcase
#undef vdefault
}
