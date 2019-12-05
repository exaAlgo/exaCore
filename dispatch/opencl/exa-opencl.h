#ifndef _EXA_OPENCL_H_
#define _EXA_OPENCL_H_

#include "exa-impl.h"

typedef struct exaOpenCLHandle_private *exaOpenCLHandle;
typedef struct exaOpenCLProgram_private *exaOpenCLProgram;
typedef struct exaOpenCLKernel_private *exaOpenCLKernel;
typedef struct exaOpenCLVector_private *exaOpenCLVector;
//
// Meta info
//
const char *exaOpenCLGetExt();
//
// Init and finalize the backend
//
int exaOpenCLInit(exaHandle h,const char *backend);
int exaOpenCLFinalize(exaHandle h);
//
// Create an Opencl vector
//
int exaOpenCLVectorCreate(exaVector x,exaInt size);
int exaOpenCLVectorFree(exaVector x);
int exaOpenCLVectorGetDevicePointer(exaVector x,void **ptr,
  size_t *size);
int exaOpenCLVectorRead(exaVector x ,exaScalar *out);
int exaOpenCLVectorWrite(exaVector x,exaScalar *in);
//
// Create an OpenCL program
//
int exaOpenCLProgramCreate(exaProgram p,const char *fname,
  exaSettings settings);
int exaOpenCLProgramFree(exaProgram p);
//
// Create an OpenCL kernel
//
int exaOpenCLKernelCreate(exaProgram p,const char *kernelName,
  exaKernel k);
int exaOpenCLKernelRun(exaKernel k,const int nArgs,...);
int exaOpenCLKernelFree(exaKernel k);
//
// Barrier
//
int exaOpenCLBarrier(exaHandle h);

#endif
