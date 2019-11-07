#ifndef _EXA_OPENCL_H_
#define _EXA_OPENCL_H_

#include "exa-impl.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef struct exaOpenCLHandle_private *exaOpenCLHandle;
typedef struct exaOpenCLKernel_private *exaOpenCLKernel;
typedef struct exaOpenCLVector_private *exaOpenCLVector;
//
// Init and finalize the backend
//
int exaOpenCLInit(exaHandle h,const char *backend);
int exaOpenCLFinalize(exaHandle h);
//
// create an opencl vector
//
int exaOpenCLVectorCreate(exaVector x,exaInt size);
#endif
