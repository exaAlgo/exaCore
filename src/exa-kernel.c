#include "exa-impl.h"
#include "exa-memory.h"

#include <stdio.h>
#include <stdarg.h>

int exaKernelCreate(exaProgram p,const char *kernelName,exaKernel *k_,int nArgs,...){
  exaHandle h;
  exaProgramGetHandle(p,&h);

  exaMalloc(1,k_);
  exaKernel k=*k_;
  k->nArgs=nArgs;

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

int setKernelArgFromArgList(exaKernelArg arg,va_list argList,exaDataType t){
  void *ptr=NULL;
  size_t size=0;

  exaVector val;
  exaScalar val1;
  exaULong val2;
  exaUInt val3;

  switch(t){
    case exaVector_t:
      val=va_arg(argList,exaVector);
      exaVectorGetDevicePointer(val,&ptr,&size);
      break;
    case exaScalar_t:
      val1=va_arg(argList,exaScalar);
      ptr=(void*)&val1;
      size=sizeof(exaScalar);
      break;
    case exaULong_t:
      val2=va_arg(argList,exaULong);
      ptr=(void*)&val2;
      size=sizeof(exaULong);
      break;
    case exaUInt_t:
      val3=va_arg(argList,exaUInt);
      ptr=(void*)&val3;
      size=sizeof(exaUInt);
      break;
    default:
      break;
  }

  arg->arg=ptr;
  arg->size=size;

  return 0;
}
int exaKernelRun(exaKernel k,...){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  int nArgs=k->nArgs;

  exaKernelArg args;
  exaMalloc(nArgs,&args);

  va_list argList;
  va_start(argList,k);

  int i;
  for(i=0;i<nArgs;i++){
    setKernelArgFromArgList(&args[i],argList,k->args[i]);
  }

  exaFree(args);
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
