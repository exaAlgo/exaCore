#ifndef _EXA_OCCA_H_
#define _EXA_OCCA_H_

#include "exa-impl.h"

typedef struct exaOccaHandle_private *exaOccaHandle;
typedef struct exaOccaProgram_private *exaOccaProgram;
typedef struct exaOccaKernel_private *exaOccaKernel;
typedef struct exaOccaVector_private *exaOccaVector;
//
// Meta info
//
const char *exaOccaGetExt();
//
// Init and finalize the backend
//
int exaOccaInit(exaHandle h,const char *backend);
int exaOccaFinalize(exaHandle h);
//
// Create an Occa vector
//
int exaOccaVectorCreate(exaVector x,exaInt size);
int exaOccaVectorFree(exaVector x);
int exaOccaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size);
int exaOccaVectorRead(exaVector x ,exaScalar *out);
int exaOccaVectorWrite(exaVector x,exaScalar *in);
//
// Create an Occa program
//
int exaOccaProgramCreate(exaProgram p,const char *fname,
  exaSettings settings);
int exaOccaProgramFree(exaProgram p);
//
// Create an Occa kernel
//
int exaOccaKernelCreate(exaProgram p,const char *kernelName,
  exaKernel k);
int exaOccaKernelRun(exaKernel k,const int nArgs,va_list args);
int exaOccaKernelFree(exaKernel k);
//
// Barrier
//
int exaOccaBarrier(exaHandle h);

#endif
