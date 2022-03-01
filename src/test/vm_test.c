#include "test.h"

#include "../vm.h"
#include "../memory.h"

test_func(vm_fac) {
    /*
     # hg code:
     fn fac(n, acc) {
        if n <= 1 {
            return acc
        } else {
            return fac(n-1, acc*n)
        }
     }
     return fac(5, 1)

     # hg vm ins:
     main:
     loadc R0 consts[2]
     loadc R1 consts[0]
     loadc R2 consts[1]
     call  R0 2 1 ; R0=fac(5, 1)
     return

     fac: ; R1=n, R2=acc
     loadc       R3 consts[1] ; R3=1
     gt          R3 R1 R3     ; R3=R1>R3  if n>1
     jumpfalse   R3 LELSE
     move        R0 R2
     return
     LELSE:
     move        R3 R1        ; n
     loadc       R4 consts[1] ; R4=1
     sub         R1 R3 R4     ; R1=n-1
     mul         R2 R2 R3     ; R2=acc*n
     call        R0 2 1       ; call fac
     return
     --constants table--
     0    -> 5
     1    -> 1
     2    -> fac
     */

#define fac_ptr 5
    struct hg_ins codes[] = {
        // main:
        [0] = {.op = HG_OP_LOADC, .as.load = {0, 2}},
        [1] = {.op = HG_OP_LOADC, .as.load = {1, 0}},
        [2] = {.op = HG_OP_LOADC, .as.load = {2, 1}},
        [3] = {.op = HG_OP_CALL, .as.call = {0, 2, 1}},
        [4] = {.op = HG_OP_RETURN},
        // fac:
        [fac_ptr] = {.op = HG_OP_LOADC, .as.load = {3, 1}},
        [6]       = {.op = HG_OP_GT, .as.ari = {3, 1, 3}},
        [7]       = {.op = HG_OP_JUMPFALSE, .as.jump = {3, 3 /* pc+=3 */}},
        [8]       = {.op = HG_OP_MOVE, .as.move = {0, 2}},
        [9]       = {.op = HG_OP_RETURN},
        //  else:
        [10] = {.op = HG_OP_MOVE, .as.move = {3, 1}},
        [11] = {.op = HG_OP_LOADC, .as.load = {4, 1}},
        [12] = {.op = HG_OP_SUB, .as.ari = {1, 3, 4}},
        [13] = {.op = HG_OP_MUL, .as.ari = {2, 2, 3}},
        [14] = {.op = HG_OP_CALL, .as.call = {0, 2, 1}},
        [15] = {.op = HG_OP_RETURN},
    };

    struct hg_function entry, fac;

    struct hg_value constants[] = {
        [0] = {.as.i = 1},
        [1] = {.as.i = 1},
        [2] = {.as.func = &fac},
    };
    int64_t* input = &constants[0].as.i;

    entry = (struct hg_function){
        .num_args      = 0,
        .num_returns   = 0,
        .constants     = constants,
        .num_constants = sizeof(constants) / sizeof(struct hg_value),
        .code          = codes,
        .size_code     = sizeof(codes) / sizeof(struct hg_ins),
    };
    fac = (struct hg_function){
        .num_args      = 2,
        .num_returns   = 1,
        .constants     = constants,
        .num_constants = 0,
        .code          = &codes[fac_ptr],
        .size_code     = 0,
    };

    struct hg_vm_state vs = {
        .size_stack = 1 << 10,
        .stack      = hg_alloc(sizeof(struct hg_value) * (1 << 10)),
        .size_frame = 1 << 10,
        .frames     = hg_alloc(sizeof(struct hg_call_frame) * (1 << 10)),
    };

    struct hg_value ret;

    *input = 0;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 1);

    *input = 1;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 1);

    *input = 2;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 2);

    *input = 3;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 6);

    *input = 4;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 24);

    *input = 5;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 120);

    *input = 6;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 720);

    *input = 10;
    ret    = hg_vm_execute(&vs, &entry);
    test_assert(ret.as.i == 3628800);

    clock_t start = now();
    const int N   = 100000;
    for (int i = 0; i < N; i++) {
        *input = 15;
        ret    = hg_vm_execute(&vs, &entry);
        test_assert(ret.as.i == 1307674368000);
    }
    test_println("#%d factorial elapsed: %s", N, duration2str(since(start), MACROSEC));

    hg_free(vs.stack);
    hg_free(vs.frames);
}

test_main(test_vm_fac();)
