#include <stdlib.h>
#include <stdio.h>

#include "exa-impl.h"

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
//
// exaHandle: wraps exaComm, buffer and other options
//
int exaInit(exaHandle *h, exaCommExternal ce) {
  exaMalloc(1,h);
  exaHandle h_ = *h;

  // Create comm
  exaCommCreate(&h_->comm,ce);
  // Init crystal router
  exaCrystalInit(exaGetComm(h_));
  // Init the buffer
  buffer_init(&h_->buf,1024);

  // Default value for options
  h_->dbgLevel = 0;
  h_->printStat = 0;
  h_->root = 0;

  return 0;
}
int exaFinalize(exaHandle h) {
  // Finalize crystal router
  exaCrystalFinalize(exaGetComm(h));
  // Finalize communication
  exaCommDestroy(exaGetComm(h));
  // Finalize the buffer
  buffer_free(&h->buf);

  exaFree(h);

  return 0;
}

exaComm exaGetComm(exaHandle h){
  return h->comm;
}

int exaSetComm(exaHandle h,exaComm c){
  // TODO: malloc and a copy
  h->comm=c;
}

exaInt exaSize(exaHandle h){
  return exaCommSize(exaGetComm(h));
}

exaInt exaRank(exaHandle h){
  return exaCommRank(exaGetComm(h));
}

int exaScan(exaHandle h){
  return 0;
}

void exaSplit(exaHandle h,int bin){
  //TODO: need to update stuff
  exaCommSplit(exaGetComm(h),bin);
}

int exaGop(exaHandle h,void *v,exaInt size,exaDataType type,exaOp op){
  return exaCommGop(exaGetComm(h),v,size,type,op);
}

int exaReduce(exaHandle h,void *out,void *in,exaInt size,exaDataType type,exaOp op){
  return exaCommReduce(exaGetComm(h),out,in,size,type,op);
}

int exaBcast(exaHandle h,void *in,exaInt count,exaDataType type){
  return exaCommBcast(exaGetComm(h),in,count,type,h->root);
}

void exaBarrier(exaHandle h) {
  exaCommBarrier(exaGetComm(h));
}

gs_dom exaDataTypeGetGSType(exaDataType t) {
  gs_dom out;
  switch(t) {
    case EXA_INT:
      out=exaIntGSType;
      break;
    case EXA_UINT:
      out=exaIntGSType;
      break;
    case EXA_LONG:
      out=exaLongGSType;
      break;
    case EXA_ULONG:
      out=exaLongGSType;
      break;
    case EXA_SCALAR:
      out=exaScalarGSType;
      break;
    default:
      break;
  }
  return out;
}

gs_op exaOpGetGSOp(exaOp t) {
  gs_op out;
  switch(t) {
    case EXA_ADD:
      out=gs_add;
      break;
    case EXA_MAX:
      out=gs_max;
      break;
    case EXA_MIN:
      out=gs_min;
      break;
    case EXA_MUL:
      out=gs_mul;
      break;
    default:
      break;
  }
  return out;
}

MPI_Datatype exaDataTypeGetMPIType(exaDataType t) {
  MPI_Datatype out;
  switch(t) {
    case EXA_INT:
      out=exaIntMPIType;
      break;
    case EXA_UINT:
      out=exaIntMPIType;
      break;
    case EXA_LONG:
      out=exaLongMPIType;
      break;
    case EXA_ULONG:
      out=exaLongMPIType;
      break;
    case EXA_SCALAR:
      out=exaScalarMPIType;
      break;
    default:
      break;
  }
  return out;
}

MPI_Op exaOpGetMPIOp(exaOp t) {
  MPI_Op out;
  switch(t) {
    case EXA_ADD:
      out=MPI_SUM;
      break;
    case EXA_MAX:
      out=MPI_MAX;
      break;
    case EXA_MIN:
      out=MPI_MIN;
      break;
    default:
      break;
  }
  return out;
}

void exaDataTypeGetMin(exaDataType t,void *out) {
  switch(t) {
    case EXA_INT:
      *((exaInt *)out)=exaIntMIN;
      break;
    case EXA_UINT:
      *((exaUInt *)out)=exaUIntMIN;
      break;
    case EXA_LONG:
      *((exaLong *)out)=exaLongMIN;
      break;
    case EXA_ULONG:
      *((exaULong *)out)=exaULongMIN;
      break;
    case EXA_SCALAR:
      *((exaScalar *)out)=exaScalarMIN;
      break;
    default:
      break;
  }
}

void exaDataTypeGetMax(exaDataType t,void *out) {
  switch(t) {
    case EXA_INT:
      *((exaInt *)out)=exaIntMAX;
      break;
    case EXA_UINT:
      *((exaUInt *)out)=exaUIntMAX;
      break;
    case EXA_LONG:
      *((exaLong *)out)=exaLongMAX;
      break;
    case EXA_ULONG:
      *((exaULong *)out)=exaULongMAX;
      break;
    case EXA_SCALAR:
      *((exaScalar *)out)=exaScalarMAX;
      break;
    default:
      break;
  }
}
