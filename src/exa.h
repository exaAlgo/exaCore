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
// exa kernels (we can get rid of this)
//
#define EXA_KERNEL_ARGS_MAX 64
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
  exaByte_t  =5,
  exaVector_t=6
} exaDataType;
#define exaTypeGetDataType(T) T##_t

void         exaDataTypeGetMin    (exaDataType t,void *out);
void         exaDataTypeGetMax    (exaDataType t,void *out);
size_t       exaDataTypeGetSize   (exaDataType t);
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
typedef struct exaSetting_private *exaSetting;
typedef struct exaSettings_private *exaSettings;
typedef struct exaHandle_private *exaHandle;
typedef struct exaVector_private *exaVector;
typedef struct exaProgram_private *exaProgram;
typedef struct exaKernelArg_private *exaKernelArg;
typedef struct exaDim_private *exaDim;
typedef struct exaKernel_private *exaKernel;
typedef struct exaArray_private *exaArray;
typedef struct exaBuffer_private *exaBuffer;
typedef struct exaTopology_private *exaTopology;
typedef struct exaTypeInfo_private *exaTypeInfo;
typedef struct exaValue_private *exaValue;
typedef struct exaPointer_private *exaPointer;
//
// exaRegister
//
void exaRegister(int (*init)(exaHandle,const char*),const char *prefix);
//
// exaMalloc, Realloc, Calloc, Free and size of
//
int    exaMallocArray (size_t n,size_t unit,void **p);
int    exaCallocArray (size_t n,size_t unit,void **p);
int    exaReallocArray(size_t n,size_t unit,void **p);
int    exaFree        (void *p);
int    exaDestroy     (void *p);
size_t exaSizeOf      (void *p);
//
//
// exaHandle: wraps an exaComm, buffer and other options
//
int         exaInit      (exaHandle *h,exaCommExternal ce,exaSettings settings);
int         exaFinalize  (exaHandle h);
exaComm     exaGetComm   (exaHandle h);
MPI_Comm    exaGetMPIComm(exaHandle h);
struct comm exaGetGSComm (exaHandle h);
int         exaSetComm   (exaHandle h,exaComm c);
exaInt      exaSize      (exaHandle h);
exaInt      exaRank      (exaHandle h);
int         exaScan      (exaHandle h,void *out,void *in,void *buf,exaInt size,
                          exaDataType t,exaOp op);
int         exaSplit     (exaHandle h,int bin);
int         exaGop       (exaHandle h,void *v,exaInt size,exaDataType type,exaOp op);
int         exaReduce    (exaHandle h,void *out,void *in,exaInt size,exaDataType type,
                          exaOp op);
int         exaBcast     (exaHandle h,void *in,exaInt count,exaDataType type);
void        exaBarrier   (exaHandle h);
int         exaCrystalInit    (exaHandle h);
int         exaCrystalFinalize(exaHandle h);
const char *exaGetSetting(const char *settingName,exaHandle h);
int         exaSetSetting(const char *settingName,const char *value,exaHandle h);
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
int         exaCommSplit     (exaComm *c,int bin);
int         exaCommDup       (exaComm *newComm,exaComm oldComm);
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
int exaVectorGetHandle(exaVector x,exaHandle *h);
int exaVectorSetData(exaVector x,void **data);
int exaVectorGetData(exaVector x,void **data);
exaInt exaVectorGetSize(exaVector x);
int exaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size);
int exaVectorWrite(exaVector x,exaScalar *in);
int exaVectorRead (exaVector x,exaScalar *out);
int exaVectorFree(exaVector vec);
//
// exaProgram: wraps a exaProgram
//
int exaProgramCreate(exaHandle h,const char *fname,exaProgram *p);
int exaProgramGetHandle(exaProgram p,exaHandle *h);
int exaProgramSetData(exaProgram p,void **data);
int exaProgramGetData(exaProgram p,void **data);
int exaProgramFree(exaProgram p);
//
// exaDim
//
int exaDimInit(exaDim *d,exaUInt dim,size_t *global,size_t *local);
int exaDimFree(exaDim dim);
//
// exaKernel: wraps a kernel
//
int exaKernelCreate(exaProgram p,const char *kernelName,exaKernel *k,int nArgs,...);
int exaKernelRun(exaKernel k,exaDim dim,...);
int exaKernelGetHandle(exaKernel k,exaHandle *h);
int exaKernelGetData(exaKernel k,void **data);
int exaKernelSetData(exaKernel k,void **data);
int exaKernelFree(exaKernel k);
//
// exaArray
//
int exaArrayInit__(exaArray *array_,size_t unitSize,size_t nUnits,size_t align,
  const char *file,const unsigned int line);
#define exaArrayInit_(array_,unitSize,nUnits,align)\
  exaArrayInit__(array_,unitSize,nUnits,align,__FILE__,__LINE__)
#define exaArrayInit(array_,T,nUnits)\
  exaArrayInit__(array_,sizeof(T),nUnits,ALIGNOF(T),__FILE__,__LINE__)

exaInt exaArrayResize_(exaArray a,size_t max,const char *file,const unsigned int line);
#define exaArrayResize(array_,max) exaArrayResize_(array_,max,__FILE__,__LINE__)

int exaArrayTransfer(exaArray array,exaUInt destOffset,exaInt setSrc,exaComm c);
int exaArrayTransferExt(exaArray array,exaUInt *dest,exaComm c);

void  *exaArrayGetPointer(exaArray array);
exaInt exaArrayGetSize(exaArray a);
exaInt exaArrayGetMaxSize(exaArray a);
exaInt exaArraySetSize(exaArray a,size_t n);
size_t exaArrayGetUnitSize(exaArray array);
size_t exaArrayGetAlign(exaArray array);
int    exaArrayAppend(exaArray arr,void *p);
int    exaArrayBcast(exaComm c,exaInt source,exaArray arr);
int    exaArrayFree(exaArray a);
//
// exaSettings
//
int exaSettingsInit(exaSettings *settings);
int exaSettingsFree(exaSettings settings);
const char *exaSettingsGetSetting(const char *settingName,exaSettings s);
int exaSettingsSetSetting(const char *settingName,const char *value,exaSettings s);
//
// Debug routines
//
double exaGetMaxRss();
void exaPrintStack();
#endif
