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

int exaVectorFree(exaVector vec){
  exaHandle h;
  exaVectorGetHandle(vec,&h);
  h->vectorFree(vec);
  exaFree(vec);
}

#if 0
int exaVectorsEqual(exaVector x, exaVector y,
                       exaScalar tol) {
  /* Asserts:
       - size of y == size of x
  */
  assert(x->size == y->size);

  exaInt i;
  for(i = 0; i < x->size; i++) {
    assert(!isnan(x->data[i]) && !isnan(y->data[i]));
  }

  exaInt n = x->size;
  for(i = 0; i < n; i++) {
    if(fabs(x->data[i] - y->data[i]) > tol) {
      return 0;
    }
  }

  return 1;
}

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

int exaCopyVector(exaVector y, exaVector x) {
  /* Asserts:
       - size y = size x
  */
  assert(y->size >= x->size);

  exaInt n = x->size;
  exaInt i;
  for(i = 0; i < n; i++) {
    y->data[i] = x->data[i];
  }

  return 0;
}

exaScalar exaNormVector(exaVector x, exaInt p) {
  assert(x->size > 0);

  exaInt n = x->size;
  exaScalar norm = 0;

  exaInt i;
  if(p == 1) {
    for(i = 0; i < n; i++) {
      norm += fabs(x->data[i]);
    }
  } else if(p == 2) {
    for(i = 0; i < n; i++) {
      norm += x->data[i] * x->data[i];
    }
    norm = sqrt(norm);
  } else if(p == -1) {
    norm = fabs(x->data[0]);

    for(i = 1; i < n; i++) {
      if(fabs(x->data[i]) > norm) norm = fabs(x->data[i]);
    }
  }

  return norm;
}

int exaScaleVector(exaVector y, exaVector x,
                      exaScalar alpha) {
  /* asserts:
       - size x = size y
  */
  assert(x->size == y->size);

  exaInt n = x->size;
  exaInt i;
  for(i = 0; i < n; i++) {
    y->data[i] = alpha * x->data[i];
  }

  return 0;
}

int exaCreateOnesVector(exaVector *x, exaInt size) {
  exaCreateVector(x, size);

  exaInt i;
  for(i = 0; i < size; i++) {
    (*x)->data[i] = 1.;
  }

  return 0;
}

int exaCreateZerosVector(exaVector *x, exaInt size) {
  exaCreateVector(x, size);

  exaInt i;
  for(i = 0; i < size; i++) {
    (*x)->data[i] = 0.;
  }

  return 0;
}

exaScalar exaDotVector(exaVector y, exaVector x) {
  /* asserts:
       - size x = size y
  */
  assert(x->size == y->size);

  exaScalar result = 0.0;
  exaInt i;
  for(i = 0; i < x->size; i++) {
    result += x->data[i] * y->data[i];
  }

  return result;
}

exaScalar exaAbsMaxVector(exaVector x) {
  exaScalar result = 0.0;
  exaInt i;
  for(i = 0; i < x->size; i++) {
    if(fabs(x->data[i]) > result) {
      result = fabs(x->data[i]);
    }
  }

  return result;
}

exaScalar exaMaxVector(exaVector x) {
  exaScalar result = -DBL_MAX;
  exaInt i;
  for(i = 0; i < x->size; i++) {
    if(x->data[i] > result) {
      result = x->data[i];
    }
  }

  return result;
}

exaScalar exaAbsMinVector(exaVector x) {
  exaScalar result = DBL_MAX;
  exaInt i;
  for(i = 0; i < x->size; i++) {
    if(fabs(x->data[i]) < result) {
      result = fabs(x->data[i]);
    }
  }

  return result;
}

exaScalar exaMinVector(exaVector x) {
  exaScalar result = DBL_MAX;
  exaInt i;
  for(i = 0; i < x->size; i++) {
    if(x->data[i] < result) {
      result = x->data[i];
    }
  }

  return result;
}

int exaAxpbyVector(exaVector z, exaVector x,
                      exaScalar alpha,
                      exaVector y, exaScalar beta) {
  assert(z->size == x->size);
  assert(z->size == y->size);

  exaInt n = z->size;
  exaInt i;
  for(i = 0; i < n; i++) {
    z->data[i] = alpha * x->data[i] + beta * y->data[i];
  }

  return 0;
}

int exaPrintVector(exaVector x) {
  /* Asserts:
       - size x > 0
  */
  assert(x->size > 0);

  printf("(%lf", x->data[0]);
  exaInt i;
  for(i = 1; i < x->size - 1; i++) {
    printf(", %.10lf", x->data[i]);
  }

  if(x->size > 1) {
    printf(", %.10lf)", x->data[x->size - 1]);
  } else {
    printf(")");
  }

  return 0;
}
#endif
