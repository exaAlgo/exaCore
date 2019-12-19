#include "exa-impl.h"
#include "exa-memory.h"
//
// exaComm
//
int exaSetComm(exaHandle h,exaComm c){
  // TODO: malloc and a copy
  h->comm=c;
}
exaComm exaGetComm(exaHandle h){
  return h->comm;
}

int exaCommCreate(exaComm *c,exaCommExternal ce){
  exaMalloc(1,c);
  comm_init(&(*c)->gsComm,ce);

  (*c)->info.type=exaCommType;
  return 0;
}

int exaCommDestroy(exaComm c){
  if(c==NULL) return 0;

  comm_free(&c->gsComm);
  exaFree(c);
  return 0;
}

MPI_Comm exaGetMPIComm(exaHandle h){
  return exaCommGetMPIComm(exaGetComm(h));
}
MPI_Comm exaCommGetMPIComm(exaComm c){
  return  c->gsComm.c;
}

struct comm exaGetGSComm(exaHandle h) {
  return exaCommGetGSComm(exaGetComm(h));
}
struct comm exaCommGetGSComm(exaComm c){
  return c->gsComm;
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
  exaDataType t,exaOp op){
  return exaCommScan(exaGetComm(h),out,in,buf,size,t,op);
}
int exaCommScan(exaComm c,void *out,void *in,void *buf,exaInt size,
  exaDataType t,exaOp op){
  comm_scan(out,&c->gsComm,exaDataTypeGetGSType(t),exaOpGetGSOp(op),in,size,buf);
}

int exaSplit(exaHandle h,int bin){
  exaComm c=exaGetComm(h);
  exaCommSplit(&c,bin);
  exaSetComm(h,c);
}
int exaCommSplit(exaComm *c_,int bin){
  exaComm c=*c_;

  MPI_Comm local;
  exaInt id = exaCommRank(c);
  MPI_Comm_split(c->gsComm.c,bin,id,&local);

  exaCommCrystalFinalize(c);
  exaCommDestroy(c);

  exaCommCreate(c_,local);
  MPI_Comm_free(&local);
  exaCommCrystalInit(*c_);
}

int exaCommDup(exaComm *newComm,exaComm oldComm){
  MPI_Comm local;
  MPI_Comm_dup(oldComm->gsComm.c,&local);

  exaCommCreate(newComm,local);
  MPI_Comm_free(&local);
  exaCommCrystalInit(*newComm);
}

int exaGop(exaHandle h,void *v,exaInt size,exaDataType type,exaOp op){
  return exaCommGop(exaGetComm(h),v,size,type,op);
}
int exaCommGop(exaComm c,void *v,exaInt size,exaDataType type,exaOp op){
  return MPI_Allreduce(MPI_IN_PLACE,v,size,exaDataTypeGetMPIType(type),
		       exaOpGetMPIOp(op),c->gsComm.c);
}

int exaReduce(exaHandle h,void *out,void *in,exaInt size,exaDataType type,exaOp op){
  return exaCommReduce(exaGetComm(h),out,in,size,type,op);
}
int exaCommReduce(exaComm c,void *out,void *in,exaInt size,exaDataType type,exaOp op){
  if(out==in && exaCommRank(c)==0){ // we should use MPI_IN_PLACE at root
    return MPI_Reduce(MPI_IN_PLACE,out,size,exaDataTypeGetMPIType(type),
		    exaOpGetMPIOp(op),0,c->gsComm.c);
  } else{
    return MPI_Reduce(in,out,size,exaDataTypeGetMPIType(type),
		    exaOpGetMPIOp(op),0,c->gsComm.c);
  }
}

int exaBcast(exaHandle h,void *in,exaInt count,exaDataType type){
  return exaCommBcast(exaGetComm(h),in,count,type,h->root);
}
int exaCommBcast(exaComm c,void *in,exaInt count,exaDataType type,int root){
  return MPI_Bcast(in,count,exaDataTypeGetMPIType(type),root,c->gsComm.c);
}

void exaBarrier(exaHandle h) {
  h->barrier(h);
  exaCommBarrier(exaGetComm(h));
}
void exaCommBarrier(exaComm c) {
  comm_barrier(&(c->gsComm));
}
//
// exaCrystal: Crystal router functionality
//
int exaCrystalInit(exaHandle h){
  exaCommCrystalInit(exaGetComm(h));
}
int exaCommCrystalInit(exaComm c) {
  crystal_init(&(c->cr),&(c->gsComm));
  return 0;
}

int exaCrystalFinalize(exaHandle h){
  exaCommCrystalFinalize(exaGetComm(h));
}
int exaCommCrystalFinalize(exaComm c){
  crystal_free(&(c->cr));
  return 0;
}
