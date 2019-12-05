#include "exa-occa-impl.h"
#include "exa-memory.h"
//
// Create an Occa kernel
//
int exaOccaKernelCreate(exaProgram p,const char *kernelName,exaKernel k){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  exaOccaProgram op;
  exaProgramGetData(p,(void**)&op);

  k->runKernel=exaOccaKernelRun;

  exaOccaKernel ok;
  exaMalloc(1,&ok);

  exaKernelSetData(k,(void**)&ok);

  return 0;
}

int exaOccaKernelRun(exaKernel k,const int nArgs,...){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);

  exaOccaKernel ok;
  exaKernelGetData(k,(void**)&ok);

  return 0;
}

int exaOccaKernelFree(exaKernel k)
{
  exaOccaKernel ok;
  exaKernelGetData(k,(void**)&ok);

  exaFree(ok);
  ok=NULL;
  exaKernelSetData(k,(void**)&ok);

  return 0;
}
