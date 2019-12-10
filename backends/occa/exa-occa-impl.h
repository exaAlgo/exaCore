#ifndef _EXA_OCCA_IMPL_H_
#define _EXA_OCCA_IMPL_H_

#include "exa-occa.h"
#include "exa-memory.h"

#include <occa.h>

occaType exaValueToOccaType(exaValue v);

struct exaOccaHandle_private{
  occaDevice device;
};

struct exaOccaProgram_private{
  occaProperties props;
  char *fileName;
};

struct exaOccaKernel_private{
  occaKernel kernel;
};

struct exaOccaVector_private{
  occaMemory vector;
};

#endif
