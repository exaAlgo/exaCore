#include "exa-impl.h"
#include "exa-memory.h"
//
// exaComm
//
int exaCommCreate(exaComm *c,exaCommExternal ce){
  exaMalloc(1,c);
  comm_init(&(*c)->gsComm,ce);
  return 0;
}

int exaCommDestroy(exaComm c){
  comm_free(&c->gsComm);
  exaFree(c);
  return 0;
}

MPI_Comm exaCommGetMPIComm(exaComm c){
  return  c->gsComm.c;
}

struct comm exaCommGetGSComm(exaComm c){
  return c->gsComm;
}

exaInt exaCommSize(exaComm c){
  return (exaInt) c->gsComm.np;
}

exaInt exaCommRank(exaComm c){
  return (exaInt) c->gsComm.id;
}

int exaCommScan(exaComm c,void *out,void *in,void *buf,exaInt size,
  exaDataType t,exaOp op){
  comm_scan(out,&c->gsComm,exaDataTypeGetGSType(t),exaOpGetGSOp(op),in,size,buf);
}

void exaCommSplit(exaComm c,int bin){
  exaCommExternal local;
  exaInt id = exaCommRank(c);
  MPI_Comm_split(c->gsComm.c,bin,id,&local);
  exaCommCrystalFinalize(c);
  exaCommDestroy(c);
  exaCommCreate(&c,local);
  MPI_Comm_free(&local);
  exaCommCrystalInit(c);
}

int exaCommGop(exaComm c,void *v,exaInt size,exaDataType type,exaOp op){
  return MPI_Allreduce(MPI_IN_PLACE,v,size,exaDataTypeGetMPIType(type),
		       exaOpGetMPIOp(op),c->gsComm.c);
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

int exaCommBcast(exaComm c,void *in,exaInt count,exaDataType type,int root){
  return MPI_Bcast(in,count,exaDataTypeGetMPIType(type),root,c->gsComm.c);
}

void exaCommBarrier(exaComm c) {
  comm_barrier(&(c->gsComm));
}
//
// Crystal router functionality
//
int exaCommCrystalInit(exaComm c) {
  crystal_init(&(c->cr),&(c->gsComm));
  return 0;
}

int exaCommCrystalFinalize(exaComm c){
  crystal_free(&(c->cr));
  return 0;
}
//
// gs_op setup
//
int exaTopologySetup(exaLong *ids,exaUInt n,exaComm c,int unique,int verbose,exaTopology *t){
  exaMalloc(1,t);
  (*t)->topology=gs_setup(ids,n,&c->gsComm,unique,gs_auto,verbose);
  return 0;
}

int exaTopologyOp(void *v,exaDataType T,exaOp op,unsigned transpose,exaTopology t,exaBuffer buf){
  gs(v,exaDataTypeGetGSType(T),exaOpGetGSOp(op),transpose,t->topology,&buf->buf);
}
