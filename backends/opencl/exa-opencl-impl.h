#ifndef _EXA_OPENCL_IMPL_H_
#define _EXA_OPENCL_IMPL_H_

#include "exa-opencl.h"

#define exaOpenCLChk_(err,file,line) do{\
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
    case CL_DEVICE_NOT_FOUND:\
      fprintf(stderr,"%s:%d: CL_DEVICE_NOT_FOUND\n",file,line);\
      break;\
    case CL_INVALID_CONTEXT:\
      fprintf(stderr,"%s:%d: CL_INVALID_CONTEXT\n",file,line);\
      break;\
    case CL_INVALID_PROGRAM:\
      fprintf(stderr,"%s:%d: CL_INVALID_PROGRAM\n",file,line);\
      break;\
    case CL_INVALID_PROGRAM_EXECUTABLE:\
      fprintf(stderr,"%s:%d: CL_INVALID_PROGRAM_EXECUTABLE\n",file,line);\
      break;\
    case CL_INVALID_VALUE:\
      fprintf(stderr,"%s:%d CL_INVALID_VALUE\n",file,line);\
      break;\
    case CL_INVALID_BINARY:\
      fprintf(stderr,"%s:%d CL_INVALID_BINARY\n",file,line);\
      break;\
    case CL_INVALID_BUILD_OPTIONS:\
      fprintf(stderr,"%s:%d CL_INVALID_BUILD_OPTIONS\n",file,line);\
      break;\
    case CL_OUT_OF_HOST_MEMORY:\
      fprintf(stderr,"%s:%d CL_OUT_OF_HOST_MEMORY\n",file,line);\
      break;\
    case CL_INVALID_OPERATION:\
      fprintf(stderr,"%s:%d CL_INVALID_OPERATION\n",file,line);\
      break;\
    case CL_COMPILER_NOT_AVAILABLE:\
      fprintf(stderr,"%s:%d CL_COMPILER_NOT_AVAILABLE\n",file,line);\
      break;\
    case CL_BUILD_PROGRAM_FAILURE:\
      fprintf(stderr,"%s:%d CL_BUILD_PROGRAM_FAILURE\n",file,line);\
      break;\
    case CL_INVALID_KERNEL:\
      fprintf(stderr,"%s:%d CL_INVALID_KERNEL\n",file,line);\
      break;\
    case CL_INVALID_ARG_INDEX:\
      fprintf(stderr,"%s:%d CL_INVALID_ARG_INDEX\n",file,line);\
      break;\
    case CL_INVALID_ARG_VALUE:\
      fprintf(stderr,"%s:%d CL_INVALID_ARG_VALUE\n",file,line);\
      break;\
    case CL_INVALID_ARG_SIZE:\
      fprintf(stderr,"%s:%d CL_INVALID_ARG_SIZE\n",file,line);\
      break;\
    case CL_INVALID_SAMPLER:\
      fprintf(stderr,"%s:%d CL_INVALID_SAMPLER\n",file,line);\
      break;\
    case CL_INVALID_MEM_OBJECT:\
      fprintf(stderr,"%s:%d CL_INVALID_MEM_OBJECT\n",file,line);\
      break;\
    case CL_INVALID_KERNEL_NAME:\
      fprintf(stderr,"%s:%d CL_INVALID_KERNEL_NAME\n",file,line);\
      break;\
    case CL_INVALID_KERNEL_DEFINITION:\
      fprintf(stderr,"%s:%d CL_INVALID_KERNEL_DEFINITION\n",file,line);\
      break;\
    default:\
      break;\
  }\
} while(0)
#define exaOpenCLChk(err) exaOpenCLChk_(err,__FILE__,__LINE__);

#define getProgramBuildLog_(oclp,file,line) do{\
  exaHandle h;\
  exaOpenCLHandle oclh;\
  exaProgramGetHandle(oclp,&h);\
  exaHandleGetData(h,(void**)&oclh);\
  size_t len;\
  char buffer[2048];\
  clGetProgramBuildInfo(oclp->program,oclh->deviceId,CL_PROGRAM_BUILD_LOG,\
    sizeof(buffer),buffer,&len);\
  printf("%s:%d: %s\n",file,line,buffer);\
} while(0)
#define exaGetProgramBuildLog(oclp) exaGetProgramBuildLog(oclp,__FILE__,__LINE__)

struct exaOpenCLHandle_private{
  cl_platform_id platformId;
  cl_device_id deviceId;
  cl_device_type deviceType;
  cl_context context;
  cl_command_queue queue;
};

struct exaOpenCLProgram_private{
  cl_program program;
};

struct exaOpenCLKernel_private{
  cl_kernel kernel;
  size_t local;
  size_t global;
};

struct exaOpenCLVector_private{
  cl_mem data;
};

#endif
