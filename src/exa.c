#include "exa-impl.h"

#include <stdlib.h>
#include <stdio.h>
//
// exaInit
//
int exaInit(exaHandle *h, exaCommExternal ce) {
  exaMalloc(1,h);
  exaHandle h_ = *h;

  // create comms
  exaCreateComm(&h_->comm,ce);

  exaCrystalInit(h_);

  h_->dbgLevel = 0;
  h_->printStat = 0;

  return 0;
}
//
// exaFinalize
//
int exaFinalize(exaHandle h) {
  exaCrystalFinalize(h);
  exaDestroyComm(exaGetComm(h));
  exaFree(h);
  return 0;
}
//
// exaMalloc, Realloc, Calloc and Free
//
int exaMallocArray(size_t n, size_t unit, void *p) {
  int ierr = posix_memalign((void **)p,EXA_ALIGN,n*unit);
  if(ierr)
    printf("exaMallocArray Failed: %s:%d\n",__FILE__,__LINE__);
  return ierr;
}

int exaCallocArray(size_t n, size_t unit, void *p) {
  int ierr = 0;
  *(void **)p = calloc(n, unit);
  if(n && unit && !*(void **)p) {
    ierr = 1;
    printf("exaCallocArray Failed: %s:%d\n", __FILE__, __LINE__);
  }
  return ierr;
}

int exaReallocArray(size_t n, size_t unit, void *p) {
  int ierr = 0;
  *(void **)p = realloc(*(void **)p, n * unit);
  if(n && unit && !*(void **)p) {
    ierr = 1;
    printf("exaReallocArray Failed: %s:%d\n", __FILE__, __LINE__);
  }
  return ierr;
}

int exaFree(void *p) {
  free(p);
  p = NULL;
  return 0;
}
