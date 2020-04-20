#ifndef _HG_HEDGEHOG_H_
#define _HG_HEDGEHOG_H_

struct hg_value {
    enum {
        HG_VALUE_INT,
        HG_VALUE_FLOAT,
        HG_VALUE_STRING,
        HG_VALUE_BOOL,
        HG_VALUE_NIL,
        HG_VALUE_OBJECT,
    } type;
    union {
        int b;
        int i;
        double f;
        void *p;
    } v;
};

struct parser_state {
    int nerr;
    void *lval;
    const char *fname;
    int lineno;
    int tline;
};

#endif // _HG_HEDGEHOG_H_
