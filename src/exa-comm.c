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

void exaSetComm(exaHandle h,exaComm c){
  h->comm=c;
}

int exaGop(exaComm c,void *v,exaInt size,exaDataType type,exaInt op){
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

int exaReduce(exaComm c,void *out,void *in,exaInt size,exaDataType type,exaInt op){
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

int exaBcast(exaComm c,void *in,exaInt count,exaDataType type){
  return MPI_Bcast(in,count,type,0,c->gsComm.c);
}

void exaSplitComm(exaHandle h,exaComm *c,int bin){
  exaCommExternal local;
  int id = exaCommRank(*c);
  MPI_Comm_split((*c)->gsComm.c,bin,id,&local);
  exaCrystalFinalize(h);
  exaDestroyComm(*c);
  exaCreateComm(c, local);
  MPI_Comm_free(&local);
  exaCrystalInit(h, *c);
}

int exaCrystalInit(exaHandle h,exaComm c) {
  crystal_init(&(h->cr),&(c->gsComm));
  return 0;
}

#if 0
int exaCrystalTransfer(exaHandle h,int field){
  if(field == EXA_ORIGIN)
    sarray_transfer(struct exaElement_private,&(h->elementArray),origin,0,&(h->cr));
  else if(field == EXA_PROC)
    sarray_transfer(struct exaElement_private,&(h->elementArray),proc,0,&(h->cr));
  return 0;
}
#endif

int exaCrystalFinalize(exaHandle h){
  crystal_free(&(h->cr));
  return 0;
}
