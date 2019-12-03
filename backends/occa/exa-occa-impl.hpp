#ifndef _EXA_OPENCL_IMPL_H_
#define _EXA_OPENCL_IMPL_H_

#include <occa.hpp>

#include "exa-occa.h"

struct exaOpenCLHandle_private{
  occa::device device;
};

struct exaOpenCLProgram_private{
  occa::properties props;
};

struct exaOpenCLKernel_private{
  occa::kernel kernel;
};

struct exaOpenCLVector_private{
  occa::memory vector;
};

#endif
