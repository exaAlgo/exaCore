#include "exa-impl.h"
//
// exaArrayInit
//
int exaArrayInit_(exaArray *array_,size_t unitSize,size_t nUnits,const char *file,
  const unsigned int line){
  exaMalloc(1,array_);
  array_init_(&((*array_)->arr),nUnits,unitSize,file,line);
  (*array_)->arr.n=nUnits;
}
//
// exaArrayPointer
//
void *exaArrayPointer(exaArray array){
  return array->arr.ptr;
}
//
// exaArrayFree
//
int exaArrayFree(exaArray a) {
  array_free(&(a->arr));
  exaFree(a);
  return 0;
}
//
// exaArraySize
//
exaInt exaArraySize(exaArray a) {
  return (exaInt) a->arr.n;
}
