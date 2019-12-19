#include "exa-host.h"
#include "exa-memory.h"
//
// Create an Host vector
//
int exaHostVectorCreate(exaVector x,exaUInt size)
{
  exaHandle h;
  exaVectorGetHandle(x,&h);

  x->getDevicePointer=exaHostVectorGetDevicePointer;
  x->vectorRead=exaHostVectorRead;
  x->vectorWrite=exaHostVectorWrite;

  exaScalar *data;
  exaCalloc(size,&data);

  exaVectorSetData(x,(void**)&data);

  return 0;
}

int exaHostVectorGetDevicePointer(exaVector x,void **ptr,
  size_t *size)
{
  exaScalar *data;
  exaVectorGetData(x,(void**)&data);

  *ptr=(void *)data;
  *size=sizeof(exaScalar);
}

int exaHostVectorRead(exaVector x,void *out){
  exaHandle h;
  exaVectorGetHandle(x,&h);

  exaScalar *data;
  exaVectorGetData(x,(void**)&data);

  exaInt size=exaVectorGetSize(x);
  size_t unitSize=exaVectorGetUnitSize(x);
  memcpy(out,data,size*unitSize);

  return 0;
}

int exaHostVectorWrite(exaVector x,void *in){
  exaHandle h;
  exaVectorGetHandle(x,&h);

  exaScalar* data;
  exaVectorGetData(x,(void**)&data);

  exaInt size=exaVectorGetSize(x);
  size_t unitSize=exaVectorGetUnitSize(x);
  memcpy(data,in,size*unitSize);

  return 0;
}


int exaHostVectorFree(exaVector x){
  exaScalar * data;
  exaVectorGetData(x,(void**)&data);

  exaFree(data);

  return 0;
}
