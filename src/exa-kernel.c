#include "exa-impl.h"
#include "exa-memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int exaKernelCreate(exaProgram p,const char *kernelName,exaKernel *k_)
{
  exaHandle h;
  exaProgramGetHandle(p,&h);

  exaMalloc(1,k_);
  exaKernel k=*k_;

  k->handle=h;
  h->kernelCreate(p,kernelName,k);

  k->info.objectType=exaKernelObj;

  return 0;
}

int exaKernelGetHandle(exaKernel k,exaHandle *h){
  *h=k->handle;
  return 0;
}

int exaKernelSetData(exaKernel k,void **data){
  k->data=*data;
  return 0;
}

int exaKernelGetData(exaKernel k,void **data){
  *data=k->data;
  return 0;
}

int exaKernelRunN(exaKernel k,const int nArgs,...){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  va_list argList;
  va_start(argList,nArgs);

  //k->runKernel(k,nArgs,);

  va_end(argList);

  return 0;
}

int exaKernelFree(exaKernel k){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  h->kernelFree(k);

  exaFree(k);

  return 0;
}
