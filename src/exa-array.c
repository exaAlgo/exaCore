#include "exa-impl.h"
#include "exa-memory.h"
//
// exaArrayInit
//
int exaArrayInit_(exaArray *array_,size_t unitSize,size_t nUnits,const char *file,
  const unsigned int line){
  exaMalloc(1,array_);
  exaArray array=*array_;
  array_init_(&(array->arr),nUnits,unitSize,file,line);
  array->arr.n=0;
  array->unitSize=unitSize;
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
exaInt exaArrayGetMaxSize(exaArray a){
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
// exaArrayAppend
//
int exaArrayAppend(exaArray arr,void *p){
  size_t size=exaArrayGetSize(arr);
  if(size==exaArrayGetMaxSize(arr)){ // array is full, so resize
    exaArrayResize(arr,size+size/2+1);
  }
  void *ptr=exaArrayGetPointer(arr);
  size_t unitSize=exaArrayGetUnitSize(arr);
  memcpy(ptr+size*unitSize,p,unitSize);
  exaArraySetSize(arr,size+1);
}
//
// exaArrayFree
//
int exaArrayFree(exaArray a){
  array_free(&(a->arr));
  exaFree(a);
  return 0;
}
