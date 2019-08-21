#include "exa-impl.h"
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

exaInt exaCommSize(exaComm c){
  return (exaInt) c->gsComm.np;
}

exaInt exaCommRank(exaComm c){
  return (exaInt) c->gsComm.id;
}

int exaCommScan(exaComm c){
  return 0;
}

void exaCommSplit(exaComm c,int bin){
  exaCommExternal local;
  exaInt id = exaCommRank(c);
  MPI_Comm_split(c->gsComm.c,bin,id,&local);
  exaCrystalFinalize(c);
  exaCommDestroy(c);
  exaCommCreate(&c,local);
  MPI_Comm_free(&local);
  exaCrystalInit(c);
}

int exaCommGop(exaComm c,void *v,exaInt size,exaDataType type,exaInt op){
  switch(op){
    case EXA_SUM:
      MPI_Allreduce(MPI_IN_PLACE,v,size,type,MPI_SUM,c->gsComm.c);
      break;
    case EXA_MAX:
      MPI_Allreduce(MPI_IN_PLACE,v,size,type,MPI_MAX,c->gsComm.c);
      break;
    case EXA_MIN:
      MPI_Allreduce(MPI_IN_PLACE,v,size,type,MPI_MIN,c->gsComm.c);
      break;
    default:
      break;
  }
  return 0;
}

int exaCommReduce(exaComm c,void *out,void *in,exaInt size,exaDataType type,exaInt op){
  switch(op) {
    case EXA_SUM:
      MPI_Reduce(in,out,size,type,MPI_SUM,0,c->gsComm.c);
      break;
    case EXA_MAX:
      MPI_Reduce(in,out,size,type,MPI_MAX,0,c->gsComm.c);
      break;
    case EXA_MIN:
      MPI_Reduce(in,out,size,type,MPI_MIN,0,c->gsComm.c);
      break;
    default:
      break;
  }
  return 0;
}

int exaCommBcast(exaComm c,void *in,exaInt count,exaDataType type,int root){
  return MPI_Bcast(in,count,type,root,c->gsComm.c);
}
// Crystal router functionality
int exaCrystalInit(exaComm c) {
  crystal_init(&(c->cr),&(c->gsComm));
  return 0;
}

int exaCrystalFinalize(exaComm c){
  crystal_free(&(c->cr));
  return 0;
}
