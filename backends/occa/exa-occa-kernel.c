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

int exaOccaKernelRun(exaKernel k,const int nArgs,va_list args){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);

  exaOccaKernel ok;
  exaKernelGetData(k,(void**)&ok);

  exaValue  val;
  exaVector vec;
  void *occaArgs[EXA_KERNEL_ARGS_MAX];

  exaTypeInfo info;
  int i;
  for(i=0;i<nArgs;i++){
    info=va_arg(args,exaTypeInfo);
    switch(info->type){
      case exaValueType:
        val=(exaValue)info;
        exaDebug(h,"arg: %d type=exaValue  pointer=%p\n",i,val);
        break;
      case exaVectorType:
        vec=(exaVector)info;
        exaDebug(h,"arg: %d type=exaVector pointer=%p\n",i,vec);
        break;
      default:
        break;
    }
  }

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
