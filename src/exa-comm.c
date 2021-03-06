#include "exa-impl.h"
#include "exa-memory.h"
//
// exaComm
//
int exaCommSetHandle(exaComm c,exaHandle *h){
  c->h=*h;
  return 0;
}
int exaCommGetHandle(exaComm c,exaHandle *h){
  *h=c->h;
  return 0;
}

int exaCommCreate(exaComm *c,exaExternalComm ce){
  exaMalloc(1,c);

  comm_init(&(*c)->gsComm,ce);
  crystal_init(&(*c)->cr,&(*c)->gsComm);

  (*c)->info.type=exaCommType;
  return 0;
}

int exaCommDestroy(exaComm c){
  if(c==NULL) return 0;

  crystal_free(&(c->cr));
  comm_free(&c->gsComm);

  exaFree(c);
  return 0;
}

exaExternalComm exaCommGetExternalComm(exaComm c){
  return c->gsComm.c;
}

exaInt exaSize(exaHandle h){
  return exaCommSize(exaGetComm(h));
}
exaInt exaCommSize(exaComm c){
  return (exaInt) c->gsComm.np;
}

exaInt exaRank(exaHandle h){
  return exaCommRank(exaGetComm(h));
}
exaInt exaCommRank(exaComm c){
  return (exaInt) c->gsComm.id;
}

int exaScan(exaHandle h,void *out,void *in,void *buf,exaInt size,
  exaDataType t,exaOp op)
{
  return exaCommScan(exaGetComm(h),out,in,buf,size,t,op);
}
int exaCommScan(exaComm c,void *out,void *in,void *buf,
  exaInt size,exaDataType t,exaOp op)
{
  comm_scan(out,&c->gsComm,exaDataTypeGetGSType(t),
    exaOpGetGSOp(op),in,size,buf);
}

int exaSplit(exaHandle h,int bin,int rank){
  exaComm comm=exaGetComm(h);
  exaComm newComm;
  exaCommSplit(comm,bin,rank,&newComm);
  exaSetComm(h,newComm);
}
int exaCommSplit(exaComm oldComm,int bin,int rank,exaComm *newComm){
  exaExternalComm local;
#if defined(EXA_MPI)
  MPI_Comm_split(oldComm->gsComm.c,bin,rank,&local);
#else
  local=1;
#endif

  exaCommCreate(newComm,local);
#if defined(EXA_MPI)
  MPI_Comm_free(&local);
#endif
}

int exaCommDup(exaComm *newComm,exaComm oldComm){
  exaExternalComm local;
#if defined(EXA_MPI)
  MPI_Comm_dup(oldComm->gsComm.c,&local);
#else
  local=1;
#endif

  exaCommCreate(newComm,local);
#if defined(EXA_MPI)
  MPI_Comm_free(&local);
#endif
}

int exaGop(exaHandle h,void *v,exaInt size,exaDataType type,
  exaOp op)
{
  return exaCommGop(exaGetComm(h),v,size,type,op);
}
int exaCommGop(exaComm c,void *v,exaInt size,exaDataType type,
  exaOp op)
{
#if defined(EXA_MPI)
  return MPI_Allreduce(MPI_IN_PLACE,v,size,
    exaDataTypeGetMPIType(type),exaOpGetMPIOp(op),c->gsComm.c);
#endif
}

int exaReduce(exaHandle h,void *out,void *in,exaInt size,
  exaDataType type,exaOp op,int root)
{
  return exaCommReduce(exaGetComm(h),out,in,size,type,op,root);
}
int exaCommReduce(exaComm c,void *out,void *in,exaInt size,
  exaDataType type,exaOp op,int root)
{
//TODO: Broken for  EXA_MPI==0
#if defined(EXA_MPI)
  if(out==in && exaCommRank(c)==root){
    // we should use MPI_IN_PLACE at root
    return MPI_Reduce(MPI_IN_PLACE,out,size,
      exaDataTypeGetMPIType(type),exaOpGetMPIOp(op),
      root,c->gsComm.c);
  } else{
    return MPI_Reduce(in,out,size,exaDataTypeGetMPIType(type),
		  exaOpGetMPIOp(op),root,c->gsComm.c);
  }
#else
  if(out!=in)
    memcpy(out,in,size*exaDataTypeGetSize(type));
#endif
}

int exaAllReduce(exaHandle h,void *out,void *in,exaInt size,
  exaDataType type,exaOp op)
{
  return exaCommAllReduce(exaGetComm(h),out,in,size,type,op);
}
int exaCommAllReduce(exaComm c,void *out,void *in,exaInt size,
  exaDataType type,exaOp op)
{
#if defined(EXA_MPI)
  if(out==in){
    // we should use MPI_IN_PLACE at root
    return MPI_Allreduce(MPI_IN_PLACE,out,size,
      exaDataTypeGetMPIType(type),exaOpGetMPIOp(op),c->gsComm.c);
  } else{
    return MPI_Allreduce(in,out,size,exaDataTypeGetMPIType(type),
		  exaOpGetMPIOp(op),c->gsComm.c);
  }
#endif
}

int exaBcast(exaHandle h,void *in,exaInt count,exaDataType type){
  return exaCommBcast(exaGetComm(h),in,count,type,h->root);
}
int exaCommBcast(exaComm c,void *in,exaInt count,exaDataType type,
  int root)
{
#if defined(EXA_MPI)
  return MPI_Bcast(in,count,exaDataTypeGetMPIType(type),root,
    c->gsComm.c);
#endif
}

void exaBarrier(exaHandle h) {
  h->barrier(h);
  exaCommBarrier(exaGetComm(h));
}
void exaCommBarrier(exaComm c) {
  comm_barrier(&(c->gsComm));
}
