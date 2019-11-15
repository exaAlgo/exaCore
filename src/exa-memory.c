#include <stdlib.h>
#include <stdio.h>

#include "exa-impl.h"
//
// exaMalloc, Realloc, Calloc and Free
//
int exaMallocArray(size_t n,size_t unit,void **p) {
  int ierr=posix_memalign(p,EXA_ALIGN,n*unit);
  if(ierr)
    printf("exaMallocArray Failed: %s:%d\n",__FILE__,__LINE__);
  return ierr;
}

int exaCallocArray(size_t n,size_t unit,void **p) {
  int ierr = 0;
  *p = calloc(n,unit);
  if(n && unit && !*p) {
    ierr = 1;
    printf("exaCallocArray Failed: %s:%d\n", __FILE__, __LINE__);
  }
  return ierr;
}

int exaReallocArray(size_t n,size_t unit,void **p) {
  int ierr = 0;
  *p = realloc(*p,n*unit);
  if(n && unit && !*p) {
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
