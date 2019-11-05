#include "exa-impl.h"
#include "exa-memory.h"
//
// exaTopology
//
int exaTopologySetup(exaLong *ids,exaUInt n,exaComm c,int unique,int verbose,exaTopology *t){
  exaMalloc(1,t);
  (*t)->topology=gs_setup(ids,n,&c->gsComm,unique,gs_auto,verbose);
  return 0;
}

int exaTopologyOp(void *v,exaDataType T,exaOp op,unsigned transpose,exaTopology t,exaBuffer buf){
  gs(v,exaDataTypeGetGSType(T),exaOpGetGSOp(op),transpose,t->topology,&buf->buf);
}

int exaTopologyFree(exaTopology t){
  gs_free(t->topology);
  exaFree(t);
  return 0;
}
