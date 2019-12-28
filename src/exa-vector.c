#include "exa-impl.h"
#include "exa-memory.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
//
// Vector operations
//
int exaVectorCreate(exaHandle h,exaInt size,exaDataType t,
  exaVector *x_)
{
  exaMalloc(1,x_);

  exaVector x=*x_;
  if(x==NULL) return 1;

  x->handle=h;
  x->size=size;
  x->unitSize=exaDataTypeGetSize(t);

  h->vectorCreate(x,size*x->unitSize);

  x->hostData=NULL;
  x->syncStatus=exaNoneSync;

  x->info.type=exaVectorType;

  return 0;
}

int exaVectorGetHandle(exaVector x,exaHandle *h){
  *h=x->handle;
  return 0;
}

int exaVectorSetData(exaVector x,void **data){
  x->data=*data;
  return 0;
}

int exaVectorGetData(exaVector x,void **data){
  *data=x->data;
  return 0;
}

exaInt exaVectorGetSize(exaVector x){
  return x->size;
}

size_t exaVectorGetUnitSize(exaVector x){
  return x->unitSize;
}

int exaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size){
  x->getDevicePointer(x,ptr,size);
  return 0;
}

int exaVectorWrite(exaVector x,void *in){
  x->vectorWrite(x,in);
  x->syncStatus=exaDeviceSync;
  return 0;
}

int exaVectorRead(exaVector x,void **out){
  if(!x->hostData)
    exaCalloc(x->size*x->unitSize,&x->hostData);

  if(x->syncStatus==exaDeviceSync || x->syncStatus==exaNoneSync){
    x->vectorRead(x,x->hostData);
    x->syncStatus=exaBothSync;
  }

  *out=x->hostData;

  return 0;
}

int exaVectorFree(exaVector vec){
  exaHandle h;
  exaVectorGetHandle(vec,&h);
  h->vectorFree(vec);
  if(vec->hostData)
    exaFree(vec->hostData);
  exaFree(vec);
}
