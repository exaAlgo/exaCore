#include "exa-occa-impl.h"
#include "exa-memory.h"
//
// Create an Occa kernel
//
int exaOccaKernelCreate(exaProgram p,const char *kernelName,
  exaKernel k)
{
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);

  exaOccaProgram op;
  exaProgramGetData(p,(void**)&op);

  exaOccaKernel ok;
  exaMalloc(1,&ok);
  ok->kernel=occaDeviceBuildKernel(oh->device,op->fileName,
    kernelName,op->props);

  exaKernelSetData(k,(void**)&ok);
  k->runKernel=exaOccaKernelRun;

  return 0;
}

occaType exaValueToOccaType(exaValue v){
  occaType t;
  switch(v->type){
    case exaInt_t:
      t=occaInt(v->value.i);
      break;
    case exaUInt_t:
      t=occaUInt(v->value.ui);
      break;
    case exaLong_t:
      t=occaLong(v->value.l);
      break;
    case exaULong_t:
      t=occaULong(v->value.ul);
      break;
    case exaScalar_t:
      //TODO double or float?
      t=occaDouble(v->value.s);
      break;
    case exaByte_t:
      t=occaChar(v->value.b);
      break;
    default:
      break;
  }

  return t;
}

int exaOccaKernelRun(exaKernel k,const int nArgs_,va_list args){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);

  exaOccaKernel ok;
  exaKernelGetData(k,(void**)&ok);

  exaValue  val;
  exaVector vec;
  occaType occaArgs[EXA_KERNEL_ARGS_MAX];

  exaTypeInfo info;
  int nArgs=nArgs_-1;
  int i;
  for(i=0;i<nArgs;i++){
    info=va_arg(args,exaTypeInfo);
    switch(info->type){
      case exaValueType:
        val=(exaValue)info;
        exaDebug(h,"arg: %d/%d type=exaValue  pointer=%p\n",i+1,
          nArgs,val);
        occaArgs[i]=exaValueToOccaType(val);
        break;
      case exaVectorType:
        vec=(exaVector)info;
        exaDebug(h,"arg: %d/%d type=exaVector pointer=%p\n",i+1,
          nArgs,vec);
        exaOccaVector ovec;
        exaVectorGetData(vec,(void**)&ovec);
        occaArgs[i]=ovec->vector;
        break;
      default:
        exaDebug(h,"arg: %d/%d type=N/A       pointer=%p\n",i+1,
          nArgs,vec);
        exit(1);
        break;
    }
  }

#define expand01(arr,offset) arr[offset]
#define expand02(arr,offset) expand01(arr,(offset)+0),expand01(arr,(offset)+ 1)
#define expand04(arr,offset) expand02(arr,(offset)+0),expand02(arr,(offset)+ 2)
#define expand08(arr,offset) expand04(arr,(offset)+0),expand04(arr,(offset)+ 4)
#define expand16(arr,offset) expand08(arr,(offset)+0),expand08(arr,(offset)+ 8)
#define expand32(arr,offset) expand16(arr,(offset)+0),expand16(arr,(offset)+16)
#define expand64(arr,offset) expand32(arr,(offset)+0),expand32(arr,(offset)+32)

  occaKernelRunN(ok->kernel,nArgs,expand64(occaArgs,0));

#undef expand01
#undef expand02
#undef expand04
#undef expand08
#undef expand16
#undef expand32
#undef expand64

  return 0;
}

int exaOccaKernelFree(exaKernel k)
{
  exaOccaKernel ok;
  exaKernelGetData(k,(void**)&ok);

  occaFree(ok->kernel);

  exaFree(ok);
  ok=NULL;
  exaKernelSetData(k,(void**)&ok);

  return 0;
}
