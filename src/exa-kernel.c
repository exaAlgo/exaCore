#include "exa-impl.h"
#include "exa-memory.h"

#include <stdio.h>
#include <stdarg.h>

int exaKernelCreate(exaProgram p,const char *kernelName,exaKernel *k_,int nArgs,...){
  exaHandle h;
  exaProgramGetHandle(p,&h);

  exaMalloc(1,k_);
  exaKernel k=*k_;

  va_list vaList;
  int i;

  va_start(vaList,nArgs);
  for(i=0;i<nArgs;i++){
    k->args[i]=va_arg(vaList,exaDataType);
  }
  va_end(vaList);

  k->handle=h;
  h->kernelCreate(p,kernelName,k);

  return 0;
}

int exaKernelGetHandle(exaKernel k,exaHandle *h){
  *h=k->handle;
}

int exaKernelSetData(exaKernel k,void **data){
  k->data=*data;
  return 0;
}

int exaKernelGetData(exaKernel k,void **data){
  *data=k->data;
  return 0;
}

int exaKernelRun(exaKernel k,...){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  return 0;
}

int exaKernelFree(exaKernel k){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  h->kernelFree(k);

  exaFree(k);

  return 0;
}
