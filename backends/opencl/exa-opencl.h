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

int exaOpenCLInit(exaHandle h,const char *backend);
int exaOpenCLFinalize(exaHandle h);

int exaOpenCLVectorCreate(exaVector x,exaInt size);
#endif
