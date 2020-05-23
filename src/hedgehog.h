#ifndef _HG_HEDGEHOG_H_
#define _HG_HEDGEHOG_H_

struct parser_state {
    int nerr;
    void* lval;
    const char* fname;
    int lineno;
    int tline;
};

#endif // _HG_HEDGEHOG_H_
