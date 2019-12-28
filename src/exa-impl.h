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
struct exaTypeInfo_private{
  exaType type;
};
//
// exaValueType
//
struct exaValue_private{
  struct exaTypeInfo_private info;
  exaDataType type;
  union{
    exaInt i;
    exaUInt ui;
    exaLong l;
    exaULong ul;
    exaScalar s;
    exaChar b;
    exaStr str;
  } value;
};
//
// exaSettings
//
typedef struct{
  char key[BUFSIZ];
  exaValue value;
  exaLong hash;
} exaSetting;

struct exaSettings_private{
  struct exaTypeInfo_private info;
  exaHandle h;
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

  // backend specific data
  char backendName[BUFSIZ];
  void *data;

  int debug;
  int root;

  const char *(*backendExt)();
  int (*backendInit)(exaHandle h,const char *backend);
  int (*backendFinalize)(exaHandle h);

  int (*updateSettings)(exaSettings s);

  int (*vectorCreate)(exaVector x,exaUInt size);
  int (*vectorFree)(exaVector x);

  int (*programCreate)(exaProgram p,const char *fname,
    exaSettings settings);
  int (*programFree)(exaProgram p);

  int (*kernelCreate)(exaProgram p,const char *kernelName,
    exaKernel k);
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
typedef enum{
  exaHostSync=0,
  exaDeviceSync=1,
  exaBothSync=2,
  exaNoneSync=3
} exaSyncStatus;

struct exaVector_private{
  struct exaTypeInfo_private info;

  exaHandle handle;
  exaInt size;
  size_t unitSize;

  exaSyncStatus syncStatus;
  void *hostData;
  void *data;

  int (*getDevicePointer)(exaVector x,void **ptr,size_t *size);
  int (*vectorWrite)(exaVector x,void *in);
  int (*vectorRead )(exaVector x,void *out);
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
// exaKernel
//
struct exaKernel_private{
  struct exaTypeInfo_private info;

  exaHandle handle;
  int nArgs;
  exaDataType args[EXA_KERNEL_ARGS_MAX];
  int (*runKernel)(exaKernel k,const int nArgs,va_list args);
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
// exaGS
//
struct exaGS_private{
  struct exaTypeInfo_private info;

  struct gs_data *topology;
};
#endif
