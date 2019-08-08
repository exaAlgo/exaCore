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
struct exaComm_private {
  struct comm gsComm;
  struct gs_data *verticesHandle;
  exaScalar *laplacianWeights;
  buffer buf;
};
//
// exaHandle
//
struct exaHandle_private {
  exaComm global;
  exaComm local;

  exaLong nel;
  exaLong Nnodes;
  exaLong start;
  int nv;

  struct crystal cr;

  int dbgLevel;
  int printStat;

  exaHistogram histogram;
};
//
// exaHandle: Create, Destroy
//
int exaCreateHandle(exaHandle h);
int exaDestroyHandle(exaHandle h);
//
// exaVector
//
struct exaVector_private {
  exaInt size;
  exaScalar *data;
};
//
// Memory management routines
//
#define exaMalloc(n, p) exaMallocArray ((n), sizeof(**(p)), p)
#define exaCalloc(n, p) exaCallocArray ((n), sizeof(**(p)), p)
#define exaRealloc(n, p) exaReallocArray((n), sizeof(**(p)), p)
//
// Binsort
//
void exaFiedlerMinMax(exaHandle h, exaScalar *min, exaScalar *max);
void exaGlobalIdMinMax(exaHandle h, exaLong *min, exaLong *max);
exaInt exaSetFiedlerBin(exaHandle h);
exaInt exaSetGlobalIdBin(exaHandle h);
void exaAssignBins(exaHandle h, int field, buffer *buf0);
void exaTransferToBins(exaHandle h, int field, buffer *buf0);
void exaBinSort(exaHandle h, int field, buffer *buf0);
//
// HistoSort
//
void parRSBHistogramSort(exaHandle h,exaComm c,int field,buffer *buf0);

struct parRSBHistogram_private {
  exaLong *count;
  exaScalar *probes;
};

#endif
