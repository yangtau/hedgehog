/* Created by Tau on 05/02/2019 */
#ifndef _HG_HEDGEHOG_H_
#define _HG_HEDGEHOG_H_

#include "debug.h"
#include "environment.h"
#include "expression.h"
#include "function.h"
#include "interpreter.h"
#include "statement.h"
#include "value.h"

typedef struct parser_state {
  int nerr;
  void *lval;
  const char *fname;
  int lineno;
  int tline;
} parser_state;



#endif /*_HG_HEDGEHOG_H_*/
