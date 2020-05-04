#include "exa-occa-impl.h"
//
// Create an Occa vector
//
int exaOccaVectorCreate(exaVector x,exaUInt size)
{
  exaHandle h; exaOccaHandle oh;
  exaVectorGetHandle(x,&h);
  exaHandleGetData(h,(void**)&oh);

  x->getDevicePointer=exaOccaVectorGetDevicePointer;
  x->vectorRead=exaOccaVectorRead;
  x->vectorWrite=exaOccaVectorWrite;

  exaOccaVector vec; exaMalloc(1,&vec);
  vec->vector=occaDeviceMalloc(oh->device,size,NULL,occaDefault);
  exaVectorSetData(x,(void**)&vec);

  return 0;
}

int exaOccaVectorGetDevicePointer(exaVector x,void **ptr,
  size_t *size)
{
  exaOccaVector vec;
  exaVectorGetData(x,(void**)&vec);

  *ptr=&vec->vector;
  *size=sizeof(occaMemory);
}

int exaOccaVectorRead(exaVector x,void *out){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);
  exaOccaVector ov;
  exaVectorGetData(x,(void**)&ov);

  exaInt size=exaVectorGetSize(x);
  size_t unitSize=exaVectorGetUnitSize(x);
  occaCopyMemToPtr(out,ov->vector,size*unitSize,0,occaDefault);

  return 0;
}

int exaOccaVectorWrite(exaVector x,void *in){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);
  exaOccaVector ov;
  exaVectorGetData(x,(void**)&ov);

  exaInt size=exaVectorGetSize(x);
  size_t unitSize=exaVectorGetUnitSize(x);
  occaCopyPtrToMem(ov->vector,in,size*unitSize,0,occaDefault);

  return 0;
}


int exaOccaVectorFree(exaVector x){
  exaOccaVector vec;
  exaVectorGetData(x,(void**)&vec);

  occaFree((occaType*)&vec->vector);

  exaFree(vec);

  return 0;
}
