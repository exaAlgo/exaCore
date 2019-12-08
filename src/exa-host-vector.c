#include "exa-host.h"
#include "exa-memory.h"
//
// Create an Host vector
//
int exaHostVectorCreate(exaVector x,exaInt size)
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

int exaHostVectorGetDevicePointer(exaVector x,void **ptr,size_t *size){
  exaScalar *data;
  exaVectorGetData(x,(void**)&data);

  *ptr=(void *)data;
  *size=sizeof(exaScalar);
}

int exaHostVectorRead(exaVector x,exaScalar *out){
  exaHandle h;
  exaVectorGetHandle(x,&h);

  exaScalar *data;
  exaVectorGetData(x,(void**)&data);

  exaInt size=exaVectorGetSize(x);
  memcpy(out,data,size*sizeof(exaScalar));

  return 0;
}

int exaHostVectorWrite(exaVector x,exaScalar *in){
  exaHandle h;
  exaVectorGetHandle(x,&h);

  exaScalar* data;
  exaVectorGetData(x,(void**)&data);

  exaInt size=exaVectorGetSize(x);
  memcpy(data,in,size*sizeof(exaScalar));

  return 0;
}


int exaHostVectorFree(exaVector x){
  exaScalar * data;
  exaVectorGetData(x,(void**)&data);

  exaFree(data);

  return 0;
}
