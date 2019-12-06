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
  exaDataType t;
  union{
    exaInt i;
    exaUInt ui;
    exaLong l;
    exaULong ul;
    exaScalar s;
    exaByte b;
  } value;
};
//
// exaSettings
//
typedef struct{
  char key[BUFSIZ];
  char value[BUFSIZ];
  exaLong hash;
} exaSetting;

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
// exaKernel
//
struct exaKernel_private{
  struct exaTypeInfo_private info;

  exaHandle handle;
  int nArgs;
  exaDataType args[EXA_KERNEL_ARGS_MAX];
  int (*runKernel)(exaKernel k,const int nArgs,...);
  void *data;
};

#define EXA_REVERSE_SEQ()\
  63,62,61,60,\
  59,58,57,56,55,54,53,52,51,50,\
  49,48,47,46,45,44,43,42,41,40,\
  39,38,37,36,35,34,33,32,31,30,\
  29,28,27,26,25,24,23,22,21,20,\
  19,18,17,16,15,14,13,12,11,10,\
   9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define EXA_ARG_N(\
  _01,_02,_03,_04,_05,_06,_07,_08,_09,_10,\
  _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
  _21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
  _31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
  _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
  _51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
  _61,_62,_63,_NN,...) _NN

#define EXA_ARG_(...) EXA_ARG_N(__VA_ARGS__)

#define EXA_ARG_COUNT(...) \
  EXA_ARG_(__VA_ARGS__,EXA_REVERSE_SEQ())

#define EXA_KERNEL_RUN3(N,kernel,...) exaKernelRunN(kernel,N,__VA_ARGS__)
#define EXA_KERNEL_RUN2(...) EXA_KERNEL_RUN3(__VA_ARGS__)
#define EXA_KERNEL_RUN1(...) EXA_KERNEL_RUN2(__VA_ARGS__)
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
