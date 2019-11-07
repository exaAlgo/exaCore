#ifndef _EXA_OPENCL_IMPL_H_
#define _EXA_OPENCL_IMPL_H_

#include "exa-opencl.h"

struct exaOpenCLHandle_private{
  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_device_type deviceType;
  cl_context context;
  cl_command_queue queue;
};

struct exaOpenCLKernel_private{
  cl_program program;
  cl_kernel kernel;
};

#endif
