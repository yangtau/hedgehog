#ifndef _HG_LIST_H_
#define _HG_LIST_H_
#include "object.h"
#include "value.h"

//> hg_list
struct hg_list {
    struct hg_object obj;
    struct value_array arr;
};
struct hg_list* hg_list_new(int argc, struct hg_value* argv);
//< hg_list
#endif // _HG_LIST_H_

