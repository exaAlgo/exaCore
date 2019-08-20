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
//
// Fiedler fields
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
  buffer buf;
};
//
// exaHandle
//
struct exaHandle_private{
  exaComm comm;
  struct crystal cr;
  int dbgLevel;
  int printStat;
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
// Memory management routines
//
#define exaMalloc(n, p)  exaMallocArray ((n), sizeof(**(p)), p)
#define exaCalloc(n, p)  exaCallocArray ((n), sizeof(**(p)), p)
#define exaRealloc(n, p) exaReallocArray((n), sizeof(**(p)), p)

#endif
