#undef vdispatch
#undef vcase
#undef vdefault

#define vdispatch(code) goto* code2mc[code - code_base];
#define vcase(op, y)    L_##op : y L_##op##_END:
#define vdefault(x) \
    L_DEFAULT:      \
    x;              \
    L_DEFAULT_END:

static const struct {
    const void* const start;
    const size_t len;
} op2mc[255] = {
    [HG_OP_ADD]       = {&&L_HG_OP_ADD, &&L_HG_OP_ADD_END - &&L_HG_OP_ADD},
    [HG_OP_SUB]       = {&&L_HG_OP_SUB, &&L_HG_OP_SUB_END - &&L_HG_OP_SUB},
    [HG_OP_MUL]       = {&&L_HG_OP_MUL, &&L_HG_OP_MUL_END - &&L_HG_OP_MUL},
    [HG_OP_DIV]       = {&&L_HG_OP_DIV, &&L_HG_OP_DIV_END - &&L_HG_OP_DIV},
    [HG_OP_MOD]       = {&&L_HG_OP_MOD, &&L_HG_OP_MOD_END - &&L_HG_OP_MOD},
    [HG_OP_AND]       = {&&L_HG_OP_AND, &&L_HG_OP_AND_END - &&L_HG_OP_AND},
    [HG_OP_OR]        = {&&L_HG_OP_OR, &&L_HG_OP_OR_END - &&L_HG_OP_OR},
    [HG_OP_GT]        = {&&L_HG_OP_GT, &&L_HG_OP_GT_END - &&L_HG_OP_GT},
    [HG_OP_LT]        = {&&L_HG_OP_LT, &&L_HG_OP_LT_END - &&L_HG_OP_LT},
    [HG_OP_EQ]        = {&&L_HG_OP_EQ, &&L_HG_OP_EQ_END - &&L_HG_OP_EQ},
    [HG_OP_NEQ]       = {&&L_HG_OP_NEQ, &&L_HG_OP_NEQ_END - &&L_HG_OP_NEQ},
    [HG_OP_NOT]       = {&&L_HG_OP_NOT, &&L_HG_OP_NOT_END - &&L_HG_OP_NOT},
    [HG_OP_JUMPFALSE] = {&&L_HG_OP_JUMPFALSE, &&L_HG_OP_JUMPFALSE_END - &&L_HG_OP_JUMPFALSE},
    [HG_OP_JUMP]      = {&&L_HG_OP_JUMP, &&L_HG_OP_JUMP_END - &&L_HG_OP_JUMP},
    [HG_OP_CALL]      = {&&L_HG_OP_CALL, &&L_HG_OP_CALL_END - &&L_HG_OP_CALL},
    [HG_OP_RETURN]    = {&&L_HG_OP_RETURN, &&L_HG_OP_RETURN_END - &&L_HG_OP_RETURN},
    [HG_OP_MOVE]      = {&&L_HG_OP_MOVE, &&L_HG_OP_MOVE_END - &&L_HG_OP_MOVE},
    [HG_OP_LOADC]     = {&&L_HG_OP_LOADC, &&L_HG_OP_LOADC_END - &&L_HG_OP_LOADC},
    [_HG_OP_END]      = {&&L_DEFAULT, &&L_DEFAULT_END - &&L_DEFAULT},
};

// translate vm code to machine code
assert((func->machine_code == NULL && func->code_to_mc == NULL) ||
       (func->machine_code != NULL && func->code_to_mc != NULL));

void** code2mc = NULL;
if (func->machine_code == NULL) {
    // TODO: one page, macos JIT flag
    void* mc = func->machine_code =
        mmap(NULL, getpagesize(), PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE | MAP_JIT, -1, 0);
    code2mc = func->code_to_mc = hg_alloc(sizeof(void*) * func->size_code);

    for (size_t i = 0; i < func->size_code; i++) {
        enum hg_opcode op = func->code[i].op;
        memcpy(mc, op2mc[op].start, op2mc[op].len);

        code2mc[i] = mc;
        mc += op2mc[op].len;
    }
}
