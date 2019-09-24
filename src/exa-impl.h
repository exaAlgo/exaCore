#ifndef _EXA_IMPL_H_
#define _EXA_IMPL_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#ifdef EXA_DEBUG
#include <stdio.h>
#endif

#include "exa.h"
#include "exa-memory.h"

//
// Some commonly occuring fields
//
#define EXA_FIEDLER 0
#define EXA_GLOBALID 1
#define EXA_PROC 2
#define EXA_ORIGIN 3
//
// exaComm
//
struct exaComm_private{
  struct comm gsComm;
  struct crystal cr;
};
//
// exaHandle
//
struct exaHandle_private{
  exaComm comm;
  buffer buf;
  int dbgLevel;
  int printStat;
  int root;
};
//
// exaHandle: Create, Destroy
//
int exaCreateHandle (exaHandle h);
int exaDestroyHandle(exaHandle h);
//
// exaVector
//
struct exaVector_private{
  exaInt size;
  exaScalar *data;
};
//
// exaArray
//
struct exaArray_private{
  exaInt size;
  size_t unitSize;
  struct array arr;
};

#endif
