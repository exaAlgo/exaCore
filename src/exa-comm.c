#include "exa-impl.h"
//
// exaComm
//
int exaCreateComm(exaComm *c,exaCommExternal ce){
  exaMalloc(1,c);
  comm_init(&(*c)->gsComm,ce);
  buffer_init(&(*c)->buf,1024);
  return 0;
}

int exaDestroyComm(exaComm c){
  buffer_free(&c->buf);
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

exaComm exaGetComm(exaHandle h){
  return h->comm;
}

int exaSetComm(exaHandle h,exaComm c){
  // malloc and a copy
  h->comm=c;
}

int exaGop(exaHandle h,void *v,exaInt size,exaDataType type,exaInt op){
  exaComm c=exaGetComm(h);
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

int exaReduce(exaHandle h,void *out,void *in,exaInt size,exaDataType type,exaInt op){
  exaComm c=exaGetComm(h);
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

int exaBcast(exaHandle h,void *in,exaInt count,exaDataType type){
  exaComm c=exaGetComm(h);
  return MPI_Bcast(in,count,type,0,c->gsComm.c);
}

int exaCrystalInit(exaHandle h) {
  exaComm c=exaGetComm(h);
  crystal_init(&(h->cr),&(c->gsComm));
  return 0;
}

int exaCrystalFinalize(exaHandle h){
  crystal_free(&(h->cr));
  return 0;
}

void exaSplitComm(exaHandle h,int bin){
  exaComm c=exaGetComm(h);
  exaCommExternal local;
  exaInt id = exaCommRank(c);
  MPI_Comm_split(c->gsComm.c,bin,id,&local);
  exaCrystalFinalize(h);
  exaDestroyComm(c);
  exaCreateComm(&(h->comm),local);
  MPI_Comm_free(&local);
  exaCrystalInit(h);
}
