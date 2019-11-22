#include <stdlib.h>
#include <stdio.h>

#include "exa-impl.h"
#include "exa-memory.h"

static int numBackends=0;

typedef struct{
  char *prefix;
  int (*init)(exaHandle h,const char *backend);
} exaBackend;
static exaBackend backends[EXA_MAX_BACKENDS];
//
// exaRegister
//
void exaRegister(int (*init)(exaHandle,const char*),const char *prefix){
  backends[numBackends].init=init;
  exaMalloc(strlen(prefix)+1,&backends[numBackends].prefix);
  strcpy(backends[numBackends].prefix,prefix);
  numBackends++;
}
//
// exaHandle: wraps exaComm, buffer and other options
//
int exaInit(exaHandle *h_,exaCommExternal ce,exaSettings settings) {
  exaMalloc(1,h_);
  exaHandle h=*h_;

  // Point to user ettings array
  h->settings=settings;
  // get the backend
  const char *backend=exaGetSetting("backend",h);
  // Create comm
  exaCommCreate(&h->comm,ce);
  // Init crystal router
  exaCrystalInit(h);
  // Create the buffer
  exaBufferCreate(&h->buf,1024);

  // Default value for options
  h->dbgLevel = 0;
  h->root = 0;

  int i;
  for(i=0;i<numBackends;i++)
    if(strcmp(backends[i].prefix,backend)==0) backends[i].init(h,backend);

  return 0;
}

int exaFinalize(exaHandle h) {
  // Finalize the backend
  h->backendFinalize(h);
  // Finalize crystal router
  exaCrystalFinalize(h);
  // Finalize communication
  exaCommDestroy(exaGetComm(h));
  // Finalize the buffer
  exaBufferFree(h->buf);

  exaFree(h);

  return 0;
}

int exaDestroy(void *p){
  exaTypeInfo info=(exaTypeInfo)p;

  if(strcmp(info->name,"exaComm"     )==0) exaCommDestroy(p);
  if(strcmp(info->name,"exaSettings" )==0) exaSettingsFree(p);
  if(strcmp(info->name,"exaVector"   )==0) exaVectorFree(p);
  if(strcmp(info->name,"exaProgram"  )==0) exaProgramFree(p);
  if(strcmp(info->name,"exaDim"      )==0) exaDimFree(p);
  if(strcmp(info->name,"exaKernel"   )==0) exaKernelFree(p);
  if(strcmp(info->name,"exaArray"    )==0) exaArrayFree(p);
  if(strcmp(info->name,"exaBuffer"   )==0) exaBufferFree(p);
  if(strcmp(info->name,"exaTopology" )==0) exaTopologyFree(p);

  return 0;
}

int exaHandleGetData(exaHandle h,void **data){
  *data=h->data;
  return 0;
}

int exaHandleSetData(exaHandle h,void **data){
  h->data=*data;
  return 0;
}
//
// exaOp
//
gs_op exaOpGetGSOp(exaOp t)
{
  gs_op out;
  switch(t) {
    case exaAddOp:
      out=gs_add;
      break;
    case exaMaxOp:
      out=gs_max;
      break;
    case exaMinOp:
      out=gs_min;
      break;
    case exaMulOp:
      out=gs_mul;
      break;
    default:
      break;
  }
  return out;
}

MPI_Op exaOpGetMPIOp(exaOp t)
{
  MPI_Op out;
  switch(t) {
    case exaAddOp:
      out=MPI_SUM;
      break;
    case exaMaxOp:
      out=MPI_MAX;
      break;
    case exaMinOp:
      out=MPI_MIN;
      break;
    default:
      break;
  }
  return out;
}
