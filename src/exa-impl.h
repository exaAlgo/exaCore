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
  exaBuffer buf;
  void *data;

  int dbgLevel;
  int root;

  int (*backendInit)(exaHandle h,const char *backend);
  int (*backendFinalize)(exaHandle h);

  int (*vectorCreate)(exaVector x,exaInt size);
  int (*vectorFree)(exaVector x);

  int (*programCreate)(exaProgram p,const char *fname);
  int (*programFree)(exaProgram p);

  int (*kernelCreate)(exaProgram p,const char *kernelName,exaKernel k);
  int (*kernelFree)(exaKernel k);
  int (*barrier)(exaHandle h);
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
  exaHandle handle;
  exaInt size;
  void *data;

  int (*getDevicePointer)(exaVector x,void **ptr,size_t *size);
  int (*vectorWrite)(exaVector x,exaScalar *in);
  int (*vectorRead)(exaVector x,exaScalar *out);
};
//
// exaProgram
//
struct exaProgram_private{
  exaHandle handle;
  char *fname;
  void *data;
};
//
// exaKernel
//
struct exaKernelArg_private{
  void *arg;
  size_t size;
};

struct exaKernel_private{
  exaHandle handle;
  int nArgs;
  exaDataType args[EXA_KERNEL_ARGS_MAX];
  int (*runKernel)(exaKernel k,exaKernelArg args);
  void *data;
};
//
// exaArray
//
struct exaArray_private{
  size_t unitSize;
  struct array arr;
};
//
// exaBuffer
//
struct exaBuffer_private{
  buffer buf;
};
//
// exaTopology
//
struct exaTopology_private{
  struct gs_data *topology;
};
#endif
