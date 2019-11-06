#ifndef _EXA_OPENCL_H_
#define _EXA_OPENCL_H_

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef exaOpenCLHandle_private *exaOpenCLHandle;
typedef exaOpenCLKernel_private *exaOpenCLKernel;

#endif
