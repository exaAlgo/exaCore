#ifndef _EXA_IMPL_H_
#define _EXA_IMPL_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "exa.h"
//
// exaTypeInfo: every exa data type should have this.
//
struct exaTypeInfo_private {
  exaObjectType objectType;
};
//
// exaValue
//
typedef struct{
  struct exaTypeInfo_private info;
  union{
    exaInt    i;
    exaUInt   ui;
    exaLong   l;
    exaULong  ul;
    exaScalar s;
    exaByte   b;
  } value;
  exaDataType t;
} exaValue_private;
//
// exaValue
//
typedef struct{
  struct exaTypeInfo_private info;
  union{
    exaInt    *i;
    exaUInt   *ui;
    exaLong   *l;
    exaULong  *ul;
    exaScalar *s;
    exaByte   *b;
  } pointer;
  exaDataType t;
} exaPointer_private;
//
// exaSettings
//
struct exaSetting_private{
  char key[BUFSIZ];
  char value[BUFSIZ];
  exaLong hash;
};
struct exaSettings_private{
  struct exaTypeInfo_private info;
  exaArray settings;
};
//
// exaComm
//
struct exaComm_private{
  struct exaTypeInfo_private info;
  struct comm gsComm;
  struct crystal cr;
};
//
// exaHandle
//
struct exaHandle_private{
  struct exaTypeInfo_private info;

  exaComm comm;
  exaBuffer buf;
  exaSettings settings;

  void *data;

  int debug;
  int root;

  const char *(*backendExt)();

  int (*backendInit)(exaHandle h,const char *backend);
  int (*backendFinalize)(exaHandle h);

  int (*vectorCreate)(exaVector x,exaInt size);
  int (*vectorFree)(exaVector x);

  int (*programCreate)(exaProgram p,const char *fname,exaSettings settings);
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
  struct exaTypeInfo_private info;

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
  struct exaTypeInfo_private info;

  exaHandle handle;
  char *fname;
  void *data;
};
//
// exaDim
//
struct exaDim_private{
  struct exaTypeInfo_private info;

  size_t global[3];
  size_t local[3];
  exaUInt dim;
};
//
// exaKernel
//
struct exaKernelArg_private{
  void *arg;
  size_t size;
};

struct exaKernel_private{
  struct exaTypeInfo_private info;

  exaHandle handle;
  int nArgs;
  exaDataType args[EXA_KERNEL_ARGS_MAX];
  int (*runKernel)(exaKernel k,exaDim dim,exaKernelArg args);
  void *data;
};
//
// exaArray
//
struct exaArray_private{
  struct exaTypeInfo_private info;

  size_t unitSize;
  struct array arr;
  size_t align;
};
//
// exaBuffer
//
struct exaBuffer_private{
  struct exaTypeInfo_private info;

  buffer buf;
};
//
// exaTopology
//
struct exaTopology_private{
  struct exaTypeInfo_private info;

  struct gs_data *topology;
};
#endif
