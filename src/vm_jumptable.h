#undef vdispatch
#undef vcase
#undef vdefault

#define vdispatch(code) goto* jumptable[code->op];
#define vcase(_op, x) \
    L_##_op : x;      \
    goto* jumptable[pc->op];
#define vdefault(x) \
    L_DEFAULT:      \
    x

static const void* const jumptable[255] = {
    [HG_OP_ADD]       = &&L_HG_OP_ADD,
    [HG_OP_SUB]       = &&L_HG_OP_SUB,
    [HG_OP_MUL]       = &&L_HG_OP_MUL,
    [HG_OP_DIV]       = &&L_HG_OP_DIV,
    [HG_OP_MOD]       = &&L_HG_OP_MOD,
    [HG_OP_AND]       = &&L_HG_OP_AND,
    [HG_OP_OR]        = &&L_HG_OP_OR,
    [HG_OP_GT]        = &&L_HG_OP_GT,
    [HG_OP_LT]        = &&L_HG_OP_LT,
    [HG_OP_EQ]        = &&L_HG_OP_EQ,
    [HG_OP_NEQ]       = &&L_HG_OP_NEQ,
    [HG_OP_NOT]       = &&L_HG_OP_NOT,
    [HG_OP_JUMPFALSE] = &&L_HG_OP_JUMPFALSE,
    [HG_OP_JUMP]      = &&L_HG_OP_JUMP,
    [HG_OP_CALL]      = &&L_HG_OP_CALL,
    [HG_OP_RETURN]    = &&L_HG_OP_RETURN,
    [HG_OP_MOVE]      = &&L_HG_OP_MOVE,
    [HG_OP_LOADC]     = &&L_HG_OP_LOADC,
    [_HG_OP_END]      = &&L_DEFAULT,
};
