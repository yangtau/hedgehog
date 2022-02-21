#ifndef HG_VM_H_
#define HG_VM_H_

#include "common.h"

//>----- code
enum hg_opcode {
    // arithmetic
    HG_OP_ADD = 0, // add       RT RX RY   RT = RX+RY
    HG_OP_SUB,     // sub       RT RX RY   RT = RX-RY
    HG_OP_MUL,     // mul       RT RX RY   RT = RX*RY
    HG_OP_DIV,     // div       RT RX RY   RT = RX/RY
    HG_OP_MOD,     // mod       RT RX RY   RT = RX%RY
    HG_OP_AND,     // and       RT RX RY   RT = RX&&RY
    HG_OP_OR,      // or        RT RX RY   RT = RX||RY
    HG_OP_GT,      // gt        RT RX RY   RT = RX>RY
    HG_OP_LT,      // lt        RT RX RY   RT = RX<RY
    HG_OP_EQ,      // eq        RT RX RY   RT = RX==RY
    HG_OP_NEQ,     // neq       RT RX RY   RT = RX!=RY
    HG_OP_NOT,     // not       RT RX      RT = !RX

    HG_OP_JUMPFALSE, // jumpfalse RX L       if !RX goto L
    HG_OP_JUMP,      // jump      L          goto L

    HG_OP_CALL, // call      RX N M     call RX(R{X+1},...,R{X+N}); N args, M returns
    HG_OP_RETURN, // return
    HG_OP_MOVE,   // move      RX RY      RX = RY
    HG_OP_LOADC,  // loadc     RT N       RT = constants[N]
    _HG_OP_END,
};

_Static_assert(_HG_OP_END <= 256, "opcode overflow");

struct __attribute__((__packed__)) hg_ins_arithmetic {
    int32_t rt : 8;
    int32_t ra : 8;
    int32_t rb : 8;
};

struct __attribute__((__packed__)) hg_ins_jump {
    int32_t rx : 8;
    int32_t label : 16; // pc delta
};

struct __attribute__((__packed__)) hg_ins_call {
    int32_t rx : 8; // funtion
    int32_t n : 8;  // # args
    int32_t m : 8;  // # returns
};

struct __attribute__((__packed__)) hg_ins_load {
    int32_t rx : 8;
    int32_t n : 16; // offset
};

struct __attribute__((__packed__)) hg_ins_move {
    int32_t rt : 8;
    int32_t rx : 8;
};

struct __attribute__((__packed__)) hg_ins {
    enum hg_opcode op : 8;
    union {
        struct hg_ins_arithmetic ari;
        struct hg_ins_jump jump;
        struct hg_ins_call call;
        struct hg_ins_move move;
        struct hg_ins_load load;
    } as;
};

_Static_assert(sizeof(struct hg_ins) == 4,
               "sizeof(struct hg_ins) should be 4 bytes");

struct hg_value {
    union {
        int64_t i;
        struct hg_function* func;
    } as;
};

struct hg_function {
    size_t num_args;    // number of arguments
    size_t num_returns; // number of return values
    // size_t num_constants;      // number of constants
    struct hg_value* constants; // constants used by this function
    // size_t size_code;           // size of opcodes (number of instrucations)
    struct hg_ins* code; // opcodes (instrucations)
};

typedef struct hg_value* hg_stk_ptr;

struct hg_call_frame {
    struct hg_function* func;
    struct hg_ins* ret_addr;
    hg_stk_ptr base; // base[0-255] --> R0...R255
};

// Runtime...
struct hg_vm_state {
    size_t size_stack;
    hg_stk_ptr stack;
    size_t size_frame;
    struct hg_call_frame* frames;
    struct hg_call_frame* frame_cur;
    struct hg_ins* pc;
};

struct hg_value hg_vm_execute(struct hg_vm_state*, struct hg_function*);

// limits:
// register number: 256
// max locals: 256
// max jump
// max args

/*

Arguments of a function will be viewed as its locals.

`call`:
1. check the number of arguments
2. create a new frame
   - set the new stack ptr
   - save next PC as the return address
3. set `frame_cur` to the new frame: `frame_cur++`
4. set PC to the function being called

`return`:
1. restore stack_ptr
2. restore `frame_cur`: `frame_cur--` (TODO: what if we return from a entry function?)
3. restorce PC


                   stack:
stack+size_s  -->  +---------------+              
                   |               |                 frames:
                   +---------------+                 +------------+ <---- frames+size_frame
                   | ....          |                 |            |
                   +---------------+                 +------------+
                   | R1 (local1)   |                 |            |               
                   +---------------+                 +------------+ <---- frame_cur: frame of current function call
  maybe arg0  <--- | R0 (local0)   |   <----------   |  stack ptr |
                   +---------------+                 +  --------  +
                   |               |                 |  ret_addr  |
                   +---------------+                 +------------+  
                   |               |                 |  ......    |
                   +---------------+                 +------------+
                   |               |                 |            | <--- entry function
                   +---------------+                 +------------+ <--- frames
                   |               |
  stack     --->   +---------------+

 */

#endif // HG_VM_H_
