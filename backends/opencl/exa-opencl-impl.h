#ifndef _EXA_OPENCL_IMPL_H_
#define _EXA_OPENCL_IMPL_H_

#include "exa-opencl.h"

#define exaOpenCLChk_(err,file,line) do {\
  switch(err){\
    case CL_INVALID_PLATFORM:\
      fprintf(stderr,"%s:%d: CL_INVALID_PLATFORM\n",file,line);\
      break;\
    case CL_INVALID_DEVICE_TYPE:\
      fprintf(stderr,"%s:%d: CL_INVALID_DEVICE_TYPE\n",file,line);\
      break;\
    case CL_INVALID_DEVICE:\
      fprintf(stderr,"%s:%d CL_INVALID_DEVICE\n",file,line);\
      break;\
    case CL_DEVICE_NOT_AVAILABLE:\
      fprintf(stderr,"%s:%d CL_DEVICE_NOT_AVAILABLE\n",file,line);\
      break;\
    case CL_INVALID_VALUE:\
      fprintf(stderr,"%s:%d CL_INVALID_VALUE\n",file,line);\
      break;\
    case CL_DEVICE_NOT_FOUND:\
      fprintf(stderr,"%s:%d: CL_DEVICE_NOT_FOUND\n",file,line);\
      break;\
    case CL_OUT_OF_HOST_MEMORY:\
      fprintf(stderr,"%s:%d CL_OUT_OF_HOST_MEMORY\n",file,line);\
      break;\
    default:\
      break;\
  }\
} while(0)

#define exaOpenCLChk(err) exaOpenCLChk_(err,__FILE__,__LINE__);

struct exaOpenCLHandle_private{
  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_device_type deviceType;
  cl_context context;
  cl_command_queue queue;
};

struct exaOpenCLKernel_private{
  exaOpenCLHandle handle;
  cl_program program;
  cl_kernel kernel;
};

struct exaOpenCLVector_private{
  exaOpenCLHandle handle;
  cl_mem data;
};

#endif
