#ifndef _EXA_OPENCL_H_
#define _EXA_OPENCL_H_

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef struct exaOpenCLHandle_private *exaOpenCLHandle;
typedef struct exaOpenCLKernel_private *exaOpenCLKernel;

#endif
