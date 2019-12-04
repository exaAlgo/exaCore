#include "exa-occa-impl.h"
//
// Create an Opencl vector
//
int exaOccaVectorCreate(exaVector x,exaInt size)
{
  exaHandle h; exaOccaHandle oh;
  exaVectorGetHandle(x,&h);
  exaHandleGetData(h,(void**)&oh);

  x->getDevicePointer=exaOccaVectorGetDevicePointer;
  x->vectorRead=exaOccaVectorRead;
  x->vectorWrite=exaOccaVectorWrite;

  exaOccaVector vec; exaMalloc(1,&vec);
  vec->vector=occaDeviceMalloc(oh->device,size*sizeof(exaScalar),NULL,occaDefault);
  exaVectorSetData(x,(void**)&vec);

  return 0;
}

int exaOccaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size){
  exaOccaVector vec;
  exaVectorGetData(x,(void**)&vec);

  *ptr=&vec->vector;
  *size=sizeof(occaMemory);
}

int exaOccaVectorRead(exaVector x,exaScalar *out){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);
  exaOccaVector ov;
  exaVectorGetData(x,(void**)&ov);

  exaInt size=exaVectorGetSize(x);
  occaCopyMemToPtr(out,ov->vector,size*sizeof(exaScalar),0,occaDefault);

  return 0;
}

int exaOccaVectorWrite(exaVector x,exaScalar *in){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);
  exaOccaVector ov;
  exaVectorGetData(x,(void**)&ov);

  exaInt size=exaVectorGetSize(x);
  occaCopyPtrToMem(ov->vector,in,size*sizeof(exaScalar),0,occaDefault);

  return 0;
}


int exaOccaVectorFree(exaVector x){
  exaOccaVector vec;
  exaVectorGetData(x,(void**)&vec);

  occaFree(vec->vector);

  exaFree(vec);
  vec=NULL;
  exaVectorSetData(x,(void **)&vec);

  return 0;
}
