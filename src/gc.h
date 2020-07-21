#ifndef _HG_GC_H_
#define _HG_GC_H_

#include "object.h"
#include "vm.h"

void gc_set_vm(struct vm* vm);
struct hg_object* gc_alloc(size_t size);

#endif
