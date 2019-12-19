#include "exa-impl.h"
#include "exa-memory.h"
//
// exaGS
//
int exaGSSetup(exaLong *ids,exaUInt n,exaComm c,int unique,
  int verbose,exaGS *t)
{
  exaMalloc(1,t);
  (*t)->topology=gs_setup(ids,n,&c->gsComm,unique,gs_auto,verbose);
  (*t)->info.type=exaGSType;
  return 0;
}

int exaGSOp(void *v,exaDataType T,exaOp op,unsigned transpose,
  exaGS t,exaBuffer buf)
{
  gs(v,exaDataTypeGetGSType(T),exaOpGetGSOp(op),transpose,
    t->topology,&buf->buf);
  return 0;
}

int exaGSFree(exaGS t){
  if(t==NULL) return 0;

  gs_free(t->topology);
  exaFree(t);

  return 0;
}
