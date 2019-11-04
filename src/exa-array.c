#include "exa-impl.h"
#include "exa-memory.h"
//
// exaArrayInit
//
int exaArrayInit_(exaArray *array_,size_t unitSize,size_t nUnits,const char *file,
  const unsigned int line){
  exaMalloc(1,array_);
  array_init_(&((*array_)->arr),nUnits,unitSize,file,line);
  (*array_)->arr.n=nUnits;
  (*array_)->unitSize=unitSize;
}
//
// exaArrayResize
//
exaInt exaArrayResize_(exaArray a,size_t max,const char *file,const unsigned int line){
  array_resize_(&(a->arr),max,exaArrayGetUnitSize(a),file,line);
}
//
// exaArrayPointer
//
void *exaArrayGetPointer(exaArray array){
  return array->arr.ptr;
}
//
// exaArrayGetSize
//
exaInt exaArrayGetSize(exaArray a){
  return (exaInt) a->arr.n;
}
//
// exaArrayGetMax
//
exaInt exaArrayGetMax(exaArray a){
  return (exaInt) a->arr.max;
}
//
// exaArraySetSize
//
exaInt exaArraySetSize(exaArray a,size_t n){
  if(n>a->arr.max) exaArrayResize_(a,n,__FILE__,__LINE__);
  a->arr.n=n;
}
//
// exaArrayGetUnitSize
//
size_t exaArrayGetUnitSize(exaArray array){
  return array->unitSize;
}
//
// exaArrayFree
//
int exaArrayFree(exaArray a){
  array_free(&(a->arr));
  exaFree(a);
  return 0;
}
