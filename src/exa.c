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
void exaRegister(int (*init)(exaHandle,const char*),
  const char *prefix,int priority)
{
  exaBackend bend;

  bend.init=init;
  strcpy(bend.prefix,prefix);
  bend.priority=priority;

  backends[numBackends++]=bend;
}
//
// exaHandle: wraps exaComm, buffer and other options
//
int exaInit(exaHandle *h_,exaExternalComm ce,const char *backend) {
  exaMalloc(1,h_);
  exaHandle h=*h_;

  // Create comm
  exaCommCreate(&h->comm,ce);
  // TODO: Temporary fix, pass handle as an argument
  // to commCreate
  exaCommSetHandle(h->comm,&h);

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
  } else{
    h->debug=atoi(debug);
  }

  char *root=getenv("EXA_ROOT");
  if(root==NULL) h->root = 0;
  else h->root=atoi(root);

  if(exaRank(h)==0)
    exaDebug(h,"[exaInit] numBackends: %d\n",numBackends);

  //TODO: sort backends based on priority
  int i,hostI;
  for(i=0;i<numBackends;i++){
    if(strcmp(backends[i].prefix,"/host")==0) hostI=i;
    if(strcmp(backend,backends[i].prefix)!=NULL){
      backends[i].init(h,backend);
      strcpy(h->backendName,backend);
      break;
    }
  }
  if(i==numBackends){
    exaDebug(h,"Backend: %s is not registered. Using /host "
      "instead.\n",backend);
    backends[hostI].init(h,"/host");
    strcpy(h->backendName,"/host");
  }

  h->info.type=exaHandleType;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaInit]\n");

  return 0;
}

int exaDebug(exaHandle h,const char *format,...)
{
  if(!exaGetDebug(h))
    return 0;
  va_list args;
  va_start(args,format);
  fflush(stdout);
  vfprintf(stdout,format,args);
  fflush(stdout);
  va_end(args);
}

int exaFinalize(exaHandle h) {
  int rank=exaRank(h);
  if(rank==0)
    exaDebug(h,"[exaFinalize]\n");

  // Finalize the backend
  h->backendFinalize(h);
  // Finalize communication
  exaCommDestroy(exaGetComm(h));
  // Finalize the buffer
  exaBufferFree(h->buf);

  if(rank==0)
    exaDebug(h,"[/exaFinalize]\n");

  exaFree(h);

  return 0;
}

int exaSetComm(exaHandle h,exaComm c){
  // TODO: Is this correct?
  h->comm=c;
}
exaComm exaGetComm(exaHandle h){
  return h->comm;
}

exaExternalComm exaGetExternalComm(exaHandle h){
  return exaCommGetExternalComm(exaGetComm(h));
}

int exaGetDebug(exaHandle h)
{
  return h->debug;
}

int exaSetDebug(exaHandle h,int debug)
{
  if(exaRank(h)==0)
    exaDebug(h,"[exaSetDebug]\n");
  h->debug=debug;
  if(exaRank(h)==0)
    exaDebug(h,"[/exaSetDebug]\n");
}

int exaDestroy(void *p){
  if(p==NULL) return 0;

  exaTypeInfo info=(exaTypeInfo)p;
  switch(info->type){
    case exaSettingsType:
      exaSettingsFree(p);
      break;
    case exaCommType:
      exaCommDestroy(p);
      break;
    case exaVectorType:
      exaVectorFree(p);
      break;
    case exaProgramType:
      exaProgramFree(p);
      break;
    case exaKernelType:
      exaKernelFree(p);
      break;
    case exaArrayType:
      exaArrayFree(p);
      break;
    case exaBufferType:
      exaBufferFree(p);
      break;
    case exaGSType:
      exaGSFree(p);
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

const char *exaGetBackendName(exaHandle h){
  return h->backendName;
}

const char *exaGetBackendExtension(exaHandle h){
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

#if defined(EXA_MPI)
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
#endif
