#ifndef _EXA_OPENCL_H_
#define _EXA_OPENCL_H_

#include "exa-impl.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef struct exaOpenCLHandle_private *exaOpenCLHandle;
typedef struct exaOpenCLProgram_private *exaOpenCLProgram;
typedef struct exaOpenCLKernel_private *exaOpenCLKernel;
typedef struct exaOpenCLVector_private *exaOpenCLVector;
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
//
// Create an OpenCL program
//
int exaOpenCLProgramCreate(exaProgram p,const char *fname);
int exaOpenCLProgramFree(exaProgram p);
//
// Create an OpenCL kernel
//
int exaOpenCLKernelCreate(exaProgram p,const char *kernelName,exaKernel k);
int exaOpenCLKernelRun(exaKernel k,...);
int exaOpenCLKernelFree(exaKernel k);
#endif
