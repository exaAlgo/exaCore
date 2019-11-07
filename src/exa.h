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
// exa max backends
//
#define EXA_MAX_BACKENDS 32
//
// exa Memory Align
//
#define EXA_ALIGN 32
//
// exa tolerances
//

#define EXA_DP_TOL 1e-12
#define EXA_TOL EXA_DP_TOL
//
// exaAssert
//
#define exaAssert(x) do{\
  assert(x);\
} while(0)
//
// exa Operators
//
typedef enum {
  exaAddOp =0,
  exaMaxOp =1,
  exaMinOp =2,
  exaMulOp =3
} exaOp;

gs_op  exaOpGetGSOp (exaOp t);
MPI_Op exaOpGetMPIOp(exaOp t);
//
// exaDataType
//
typedef enum {
  exaInt_t   =0,
  exaUInt_t  =1,
  exaLong_t  =2,
  exaULong_t =3,
  exaScalar_t=4,
  exaByte_t  =5
} exaDataType;
#define exaTypeGetDataType(T) T##_t

void         exaDataTypeGetMin    (exaDataType t,void *out);
void         exaDataTypeGetMax    (exaDataType t,void *out);
gs_dom       exaDataTypeGetGSType (exaDataType t);
MPI_Datatype exaDataTypeGetMPIType(exaDataType t);
//
// exaCommExternal
//
typedef MPI_Comm exaCommExternal;
//
// exa Pointer types
//
typedef struct exaComm_private *exaComm;
typedef struct exaHandle_private *exaHandle;
typedef struct exaVector_private *exaVector;
typedef struct exaArray_private *exaArray;
typedef struct exaBuffer_private *exaBuffer;
typedef struct exaTopology_private *exaTopology;
//
// exaRegister
//
void exaRegister(int (*init)(exaHandle,const char*),const char *prefix);
//
// exaMalloc, Realloc, Calloc and Free
//
int exaMallocArray (size_t n,size_t unit,void *p);
int exaCallocArray (size_t n,size_t unit,void *p);
int exaReallocArray(size_t n,size_t unit,void *p);
int exaFree        (void *p);
//
// exaHandle: wraps an exaComm, buffer and other options
//
int         exaInit      (exaHandle *h,exaCommExternal ce,const char *backend);
int         exaFinalize  (exaHandle h);
exaComm     exaGetComm   (exaHandle h);
MPI_Comm    exaGetMPIComm(exaHandle h);
struct comm exaGetGSComm (exaHandle h);
int         exaSetComm   (exaHandle h,exaComm c);
exaInt      exaSize      (exaHandle h);
exaInt      exaRank      (exaHandle h);
int         exaScan      (exaHandle h,void *out,void *in,void *buf,exaInt size,
                          exaDataType t,exaOp op);
void        exaSplit     (exaHandle h,int bin);
int         exaGop       (exaHandle h,void *v,exaInt size,exaDataType type,exaOp op);
int         exaReduce    (exaHandle h,void *out,void *in,exaInt size,exaDataType type,
                          exaOp op);
int         exaBcast     (exaHandle h,void *in,exaInt count,exaDataType type);
void        exaBarrier   (exaHandle h);
int         exaCrystalInit    (exaHandle h);
int         exaCrystalFinalize(exaHandle h);
// functions related to the backend
int         exaHandleGetData(exaHandle h,void **data);
int         exaHandleSetData(exaHandle h,void **data);
//
// exaComm: wraps gslib comm, gs_op and crystal router
//
int         exaCommCreate    (exaComm *c,exaCommExternal ce);
int         exaCommDestroy   (exaComm c);
MPI_Comm    exaCommGetMPIComm(exaComm c);
struct comm exaCommGetGSComm (exaComm c);
exaInt      exaCommSize      (exaComm c);
exaInt      exaCommRank      (exaComm c);
int         exaCommScan      (exaComm c,void *out,void *in,void *buf,exaInt size,
                              exaDataType t,exaOp op);
void        exaCommSplit     (exaComm c,int bin);
int         exaCommGop       (exaComm c,void *v,exaInt size,exaDataType type,exaOp op);
int         exaCommReduce    (exaComm c,void *out,void *in,exaInt size,exaDataType type,exaOp op);
int         exaCommBcast     (exaComm c,void *in,exaInt count,exaDataType type,int root);
void        exaCommBarrier   (exaComm c);
// crystal router functionality
int         exaCommCrystalInit    (exaComm c);
int         exaCommCrystalFinalize(exaComm c);
//
// exaBuffer:
//
int exaBufferCreate(exaBuffer *buf,size_t size);
int exaBufferFree(exaBuffer buf);
//
// exaTopology: gs topology setup and gs_op
//
int exaTopologySetup(exaLong *ids,exaUInt n,exaComm c,int unique,int verbose,exaTopology *t);
int exaTopologyOp(void *v,exaDataType T,exaOp op,unsigned transpose,exaTopology t,exaBuffer buf);
int exaTopologyFree(exaTopology t);
//
// exaVector: wraps a vector. Currently just a host vector.
//
int exaVectorCreate(exaHandle h,exaInt size,exaVector *x);
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
exaScalar exaNormVector(exaVector x,exaInt p);

int exaPrintVector(exaVector x);
int exaDestroyVector(exaVector x);
//
// exaArray
//
int exaArrayInit_(exaArray *array_,size_t unitSize,size_t nUnits,const char *file,
  const unsigned int line);
#define exaArrayInit(array_,unitSize,nUnits) exaArrayInit_(array_,unitSize,nUnits,__FILE__,__LINE__)

exaInt exaArrayResize_(exaArray a,size_t max,const char *file,const unsigned int line);
#define exaArrayResize(array_,max) exaArrayResize_(array_,max,__FILE__,__LINE__)

void *exaArrayGetPointer(exaArray array);

#define exaArrayTransfer(T,array_,proc_field,cr) \
  sarray_transfer(T,&(array_->arr),proc_field,1,cr)

exaInt exaArrayGetSize(exaArray a);
exaInt exaArrayGetMaxSize(exaArray a);
exaInt exaArraySetSize(exaArray a,size_t n);
size_t exaArrayGetUnitSize(exaArray array);
int    exaArrayAppend(exaArray arr,void *p);
int    exaArrayBcast(exaComm c,exaInt source,exaArray arr);
int    exaArrayFree(exaArray a);
//
// Debug routines
//
double exaGetMaxRss();
void exaPrintStack();
#endif
