#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "exa-impl.h"
#include "exa-memory.h"

typedef struct{
  exaInt priority;
  int (*init)(exaHandle h,const char *backend);
  char prefix[BUFSIZ];
} exaBackend;

static int numBackends=0;
static exaBackend backends[EXA_MAX_BACKENDS];
//
// exaRegister
//
void exaRegister(int (*init)(exaHandle,const char*),const char *prefix,int priority){
  exaBackend bend;

  bend.init=init;
  strcpy(bend.prefix,prefix);
  bend.priority=priority;

  backends[numBackends++]=bend;
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
  char *debug=getenv("EXA_DEBUG");
  if(debug==NULL){
#if defined(EXA_DEBUG)
    h->debug=1;
#else
    h->debug=0;
#endif
  } else h->debug=atoi(debug);

  char *root=getenv("EXA_ROOT");
  if(root==NULL) h->root = 0;
  else h->root=atoi(root);

  exaDebug(h,"numBackends: %d\n",numBackends);

  //TODO: sort backends based on priority
  int i;
  for(i=0;i<numBackends;i++)
    if(strcmp(backends[i].prefix,backend)==0) backends[i].init(h,backend);

  h->info.objectType=exaHandleObj;

  return 0;
}

int exaDebug(exaHandle h,const char *format,...)
{
  if(!exaGetDebug(h)) return 0;
  va_list args;
  va_start(args,format);
  fflush(stdout);
  vfprintf(stdout,format,args);
  fflush(stdout);
  va_end(args);
}

int exaGetDebug(exaHandle h)
{
  return h->debug;
}

int exaSetDebug(exaHandle h,int debug)
{
  h->debug=debug;
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

  switch(info->objectType){
    case exaCommObj:
      exaCommDestroy(p);
      break;
    case exaSettingsObj:
      exaSettingsFree(p);
      break;
    case exaVectorObj:
      exaVectorFree(p);
      break;
    case exaProgramObj:
      exaProgramFree(p);
      break;
    case exaDimObj:
      exaDimFree(p);
      break;
    case exaKernelObj:
      exaKernelFree(p);
      break;
    case exaArrayObj:
      exaArrayFree(p);
      break;
    case exaBufferObj:
      exaBufferFree(p);
      break;
    case exaTopologyObj:
      exaTopologyFree(p);
      break;
    default:
      break;
  }

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

const char *exaBackendExtension(exaHandle h)
{
  return h->backendExt();
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
