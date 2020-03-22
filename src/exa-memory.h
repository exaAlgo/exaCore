#ifndef _EXA_MEMORT_H_
#define _EXA_MEMORY_H_
//
// exa Memory allocation
//
#define exaMalloc(n,p) exaMallocArray((n),sizeof(**(p)),(void**)p)
#define exaCalloc(n,p) exaCallocArray((n),sizeof(**(p)),(void**)p)
#define exaRealloc(n,p) exaReallocArray((n),sizeof(**(p)),(void**)p)

#endif
