#ifndef _EXA_H_
#define _EXA_H_
//
// Header for exa types
//
#include "exa-types.h"
//
// Header for gslib
//
#include "exa-gslib.h"
//
// Header for MPI
//
#include <mpi.h>
//
// exa Operators
//
#define EXA_SUM 0
#define EXA_MAX 1
#define EXA_MIN 2
#define EXA_MUL 3
//
// exa Memory Align
//
#define EXA_ALIGN 32
//
// exa tolerances
//
#define EXA_SP_TOL 1e-05
#define EXA_DP_TOL 1e-12
#define EXA_TOL EXA_DP_TOL
//
// exaCommExternal
//
typedef MPI_Datatype exaDataType;
typedef MPI_Comm exaCommExternal;
//
// exa Pointer types
//
typedef struct exaComm_private *exaComm;
typedef struct exaHandle_private *exaHandle;
typedef struct exaVector_private *exaVector;
typedef struct array *exaArray;
//
// exa: Init, Finalize
//
int exaInit(exaHandle *h, exaCommExternal ce);
int exaFinalize(exaHandle h);
//
// exaMalloc, Realloc, Calloc and Free
//
int exaMallocArray (size_t n,size_t unit,void *p);
int exaCallocArray (size_t n,size_t unit,void *p);
int exaReallocArray(size_t n,size_t unit,void *p);
int exaFree        (void *p);
//
// exa communication routines
//
exaComm exaGetComm(exaHandle h);
int     exaSetComm(exaHandle h,exaComm c);
exaInt  exaSize   (exaHandle h);
exaInt  exaRank   (exaHandle h);

int exaCrystalInit    (exaHandle h);
int exaCrystalFinalize(exaHandle h);

int exaScan  (exaHandle h);
int exaGop   (exaHandle h,void *v,exaInt size,exaDataType type,exaInt op);
int exaReduce(exaHandle h,void *out,void *in,exaInt size,exaDataType type,exaInt op);
int exaBcast (exaHandle h,void *in,exaInt count,exaDataType type);
//
// exaComm
//
int exaCreateComm(exaComm *c,exaCommExternal ce);
exaInt exaCommSize(exaComm c);
exaInt exaCommRank(exaComm c);
int exaDestroyComm(exaComm c);
void exaSplitComm(exaHandle h,int bin);
//
// exaVector operations
//
int exaCreateVector(exaVector *x,exaInt size);
int exaSetVector(exaVector x,exaScalar *array);
int exaGetVector(exaVector x,exaScalar *array);

int exaCreateRandomVector(exaVector *x,exaInt size,exaInt seed);
int exaCreateOnesVector(exaVector *x,exaInt size);
int exaCreateZerosVector(exaVector *x,exaInt size);

int exaScaleVector(exaVector y,exaVector x,exaScalar alpha);
int exaAxpbyVector(exaVector z,exaVector x,exaScalar alpha,exaVector y,exaScalar beta);

int exaVectorsEqual(exaVector x,exaVector y,exaScalar tol);
int exaCopyVector(exaVector x, exaVector y);
exaScalar exaDotVector(exaVector x, exaVector y);
exaScalar exaAbsMaxVector(exaVector x);
exaScalar exaMaxVector(exaVector x);
exaScalar exaAbsMinVector(exaVector x);
exaScalar exaMinVector(exaVector x);
exaScalar exaNormVector(exaVector x, exaInt p);

int exaPrintVector(exaVector x);
int exaDestroyVector(exaVector x);
//
// exaArray: just a gslib array
//
#define exaArrayCreate(T,array,n) \
  array_init(T,array,n);
#define exaArrayGetPointer(T,array) \
  ((T *) array->ptr)
#define exaArrayGetElement(T,array,i) \
  ((T *) array->ptr)[i]
#define exaArrayTransfer(T,array,proc_field,cr) \
  sarray_transfer(T,array,proc_field,1,cr);

exaInt exaArraySize(exaArray a);
int    exaArrayFree(exaArray a);
//
// Debug routines
//
double exaGetMaxRss();
void exaPrintStack();
#endif
