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

void exaOpenCLChk(cl_int err);
int exaOpenCLInit(exaHandle h);
int exaOpenCLFinalize(exaHandle h);

int vectorCreateOpenCL(exaVector x,exaInt size);
#endif
