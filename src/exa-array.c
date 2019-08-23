#include "exa-impl.h"
//
// exaArrayFree
//
int exaArrayFree(exaArray a) {
  array_free(a);
  exaFree(a);
  return 0;
}
//
// exaArraySize
//
exaInt exaArraySize(exaArray a) {
  return (exaInt) a->n;
}
