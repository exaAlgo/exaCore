#ifndef _EXA_H_
#define _EXA_H_
//
// Header for MPI
//
#include <mpi.h>
//
// Header for gslib
//
#include "exa-gslib.h"
//
// Header for exa types
//
#include "exa-types.h"
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
  exaChar_t  =5,
  exaStr_t   =6
} exaDataType;

void         exaDataTypeGetMin    (exaDataType t,void *out);
void         exaDataTypeGetMax    (exaDataType t,void *out);
size_t       exaDataTypeGetSize   (exaDataType t);
gs_dom       exaDataTypeGetGSType (exaDataType t);
MPI_Datatype exaDataTypeGetMPIType(exaDataType t);
//
// exaType
//
typedef enum {
  exaValueType    = 1,
  exaCommType     = 2,
  exaSettingsType = 3,
  exaHandleType   = 4,
  exaVectorType   = 5,
  exaProgramType  = 6,
  exaKernelType   = 7,
  exaArrayType    = 8,
  exaBufferType   = 9,
  exaGSType       =10
} exaType;

typedef struct exaTypeInfo_private *exaTypeInfo;
typedef struct exaValue_private *exaValue;
typedef struct exaComm_private *exaComm;
typedef struct exaSettings_private *exaSettings;
typedef struct exaHandle_private *exaHandle;
typedef struct exaVector_private *exaVector;
typedef struct exaProgram_private *exaProgram;
typedef struct exaKernel_private *exaKernel;
typedef struct exaArray_private *exaArray;
typedef struct exaBuffer_private *exaBuffer;
typedef struct exaGS_private *exaGS;
//
// exaCommExternal
//
typedef MPI_Comm exaCommExternal;
//
// exaRegister
//
void exaRegister(int (*init)(exaHandle,const char*),
  const char *prefix,int priority);
//
// exaMalloc, Realloc, Calloc, Free and size of
//
int exaMallocArray (size_t n,size_t unit,void **p);
int exaCallocArray (size_t n,size_t unit,void **p);
int exaReallocArray(size_t n,size_t unit,void **p);
int exaFree        (void *p);
int exaDestroy     (void *p);
//
// exaHandle: wraps an exaComm, buffer and other options
//
int exaInit(exaHandle *h,exaCommExternal ce,const char *backend);
int exaFinalize(exaHandle h);
int exaHandleGetData(exaHandle h,void **data);
int exaHandleSetData(exaHandle h,void **data);
// functions to query the backend
const char *exaGetBackendName(exaHandle h);
const char *exaGetBackendExtension(exaHandle h);
// communication
exaComm     exaGetComm   (exaHandle h);
MPI_Comm    exaGetMPIComm(exaHandle h);
struct comm exaGetGSComm (exaHandle h);
int         exaSetComm   (exaHandle h,exaComm c);
exaInt      exaSize      (exaHandle h);
exaInt      exaRank      (exaHandle h);
// scan, gop and bcast
int exaScan(exaHandle h,void *out,void *in,void *buf,exaInt size,
  exaDataType type,exaOp op);
int exaGop(exaHandle h,void *v,exaInt size,exaDataType type,exaOp op);
int exaReduce(exaHandle h,void *out,void *in,exaInt size,
  exaDataType type,exaOp op);
int exaBcast(exaHandle h,void *in,exaInt count,exaDataType type);
// crystal transfer
int exaCrystalInit    (exaHandle h);
int exaCrystalFinalize(exaHandle h);
void exaBarrier(exaHandle h);
int exaSplit(exaHandle h,int bin);
// debug
int exaDebug   (exaHandle h,const char *format,...);
int exaGetDebug(exaHandle h);
int exaSetDebug(exaHandle h,int debug);
//
// exaComm: wraps gslib comm, gs_op and crystal router
//
int         exaCommCreate    (exaComm *c,exaCommExternal ce);
int         exaCommDestroy   (exaComm c);
MPI_Comm    exaCommGetMPIComm(exaComm c);
struct comm exaCommGetGSComm (exaComm c);
exaInt      exaCommSize      (exaComm c);
exaInt      exaCommRank      (exaComm c);
int         exaCommScan      (exaComm c,void *out,void *in,void *buf,
  exaInt size,exaDataType t,exaOp op);
int         exaCommSplit     (exaComm *c,int bin);
int         exaCommDup       (exaComm *newComm,exaComm oldComm);
int         exaCommGop       (exaComm c,void *v,exaInt size,
  exaDataType type,exaOp op);
int         exaCommReduce    (exaComm c,void *out,void *in,
  exaInt size,exaDataType type,exaOp op);
int         exaCommBcast     (exaComm c,void *in,exaInt count,
  exaDataType type,int root);
void        exaCommBarrier   (exaComm c);
// crystal router functionality
int         exaCommCrystalInit    (exaComm c);
int         exaCommCrystalFinalize(exaComm c);
//
// exaSettings
//
int exaSettingsInit(exaHandle h,const char *fname,
  exaSettings *settings);
int exaSettingsFree(exaSettings settings);
int exaSettingsGetHandle(exaSettings s,exaHandle *h);
exaValue exaSettingsGetValue(const char *settingName,exaSettings s);
int exaSettingsGet(void *out,const char *settingName,
  exaSettings s);
int exaSettingsSet(const char *settingName,exaValue value,
  exaSettings s);
const char *exaSettingsIterateKeys(const char *startsWith,
  exaSettings s);
//
// exaBuffer:
//
int exaBufferCreate(exaBuffer *buf,size_t size);
int exaBufferFree(exaBuffer buf);
//
// exaGS: gs topology setup and gs_op
//
int exaGSSetup(exaLong *ids,exaUInt n,exaComm c,int unique,
  int verbose,exaGS *t);
int exaGSOp(void *v,exaDataType T,exaOp op,unsigned transpose,
  exaGS t,exaBuffer buf);
int exaGSFree(exaGS t);
//
// exaVector: wraps a vector. Currently just a host vector.
//
int exaVectorCreate(exaHandle h,exaInt size,exaDataType type,
  exaVector *x);
int exaVectorGetHandle(exaVector x,exaHandle *h);
int exaVectorSetData(exaVector x,void **data);
int exaVectorGetData(exaVector x,void **data);
exaInt exaVectorGetSize(exaVector x);
size_t exaVectorGetUnitSize(exaVector x);
int exaVectorGetDevicePointer(exaVector x,void **ptr,size_t *size);
int exaVectorWrite(exaVector x,void *in);
int exaVectorRead (exaVector x,void *out);
int exaVectorFree(exaVector vec);
//
// exaProgram: wraps a exaProgram
//
int exaProgramCreate(exaHandle h,const char *fname,
  exaSettings settings,exaProgram *p);
int exaProgramGetHandle(exaProgram p,exaHandle *h);
int exaProgramSetData(exaProgram p,void **data);
int exaProgramGetData(exaProgram p,void **data);
int exaProgramFree(exaProgram p);
//
// exaValue
//
int exaValueCreate(exaDataType t,void *value,exaValue *v);
int exaValueGetNative(void *out,exaValue v);
int exaValueEq(exaValue val1,exaValue val2);
//TODO change arguments to standard c types
exaValue getExaInt   (const exaInt    i);
exaValue getExaUInt  (const exaUInt   i);
exaValue getExaLong  (const exaLong   i);
exaValue getExaULong (const exaULong  i);
exaValue getExaScalar(const exaScalar i);
exaValue getExaChar  (const exaChar   i);
exaValue getExaStr   (const char *    i);
int exaValueFree(exaValue v);
//
// exaKernel: wraps a kernel
//
int exaKernelCreate(exaProgram p,const char *kernelName,exaKernel *k);
int exaKernelRunN(exaKernel k,const int nArgs,...);

#define EXA_REVERSE_SEQ()\
  64,63,62,61,60,\
  59,58,57,56,55,54,53,52,51,50,\
  49,48,47,46,45,44,43,42,41,40,\
  39,38,37,36,35,34,33,32,31,30,\
  29,28,27,26,25,24,23,22,21,20,\
  19,18,17,16,15,14,13,12,11,10,\
   9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define EXA_ARG_N(\
  _01,_02,_03,_04,_05,_06,_07,_08,_09,_10,\
  _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,\
  _21,_22,_23,_24,_25,_26,_27,_28,_29,_30,\
  _31,_32,_33,_34,_35,_36,_37,_38,_39,_40,\
  _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,\
  _51,_52,_53,_54,_55,_56,_57,_58,_59,_60,\
  _61,_62,_63,_64,_NN,...) _NN

#define EXA_ARG_(...) EXA_ARG_N(__VA_ARGS__)

#define EXA_ARG_COUNT(...) \
  EXA_ARG_(__VA_ARGS__,EXA_REVERSE_SEQ())

#define EXA_KERNEL_RUN3(N,kernel,...) exaKernelRunN(kernel,N,__VA_ARGS__)
#define EXA_KERNEL_RUN2(...) EXA_KERNEL_RUN3(__VA_ARGS__)
#define EXA_KERNEL_RUN1(...) EXA_KERNEL_RUN2(__VA_ARGS__)
#define exaKernelRun(...) EXA_KERNEL_RUN1(EXA_ARG_COUNT(__VA_ARGS__),__VA_ARGS__)
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
// Debug routines
//
double exaGetMaxRss();
void exaPrintStack();
#endif
