#include "exa-impl.h"
#include "exa-memory.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>
//
// Vector operations
//
int exaVectorCreate(exaHandle h,exaInt size,exaVector *x_){
  exaMalloc(1,x_);

  exaVector x=*x_;
  if(x==NULL) return 1;

  x->handle=h;
  x->size = size;

  h->vectorCreate(x,size);

  h->info.type=exaVectorType;

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

int exaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size){
  x->getDevicePointer(x,ptr,size);
  return 0;
}

int exaVectorWrite(exaVector x,exaScalar *in){
  return x->vectorWrite(x,in);
}

int exaVectorRead(exaVector x,exaScalar *out){
  return x->vectorRead(x,out);
}

int exaVectorFree(exaVector vec){
  exaHandle h;
  exaVectorGetHandle(vec,&h);
  h->vectorFree(vec);
  exaFree(vec);
}

#if 0
int exaSetVector(exaVector x, exaScalar *array) {
  memcpy(x->data, array, sizeof(exaScalar) * (size_t)x->size);
  return 0;
}

int exaDestroyVector(exaVector x) {
  if(x->data) {
    exaFree(x->data);
  }

  if(x) {
    exaFree(x);
  }

  return 0;
}
#endif
