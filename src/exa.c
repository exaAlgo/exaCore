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
//
// exaDataType
//
size_t exaDataTypGetSize    (exaDataType t){
  size_t size;
  switch(t){
    case exaInt_t:
      size=sizeof(exaInt);
      break;
    case exaUInt_t:
      size=sizeof(exaUInt);
      break;
    case exaLong_t:
      size=sizeof(exaLong);
      break;
    case exaULong_t:
      size=sizeof(exaULong);
      break;
    case exaScalar_t:
      size=sizeof(exaScalar);
      break;
    case exaByte_t:
      size=sizeof(exaByte);
      break;
    default:
      break;
  }
}

gs_dom exaDataTypeGetGSType(exaDataType t) {
  gs_dom out;
  switch(t) {
    case exaInt_t:
      out=exaIntGSType;
      break;
    case exaUInt_t:
      out=exaIntGSType;
      break;
    case exaLong_t:
      out=exaLongGSType;
      break;
    case exaULong_t:
      out=exaLongGSType;
      break;
    case exaScalar_t:
      out=exaScalarGSType;
      break;
    default:
      break;
  }
  return out;
}

gs_op exaOpGetGSOp(exaOp t) {
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

MPI_Datatype exaDataTypeGetMPIType(exaDataType t) {
  MPI_Datatype out;
  switch(t) {
    case exaInt_t:
      out=exaIntMPIType;
      break;
    case exaUInt_t:
      out=exaIntMPIType;
      break;
    case exaLong_t:
      out=exaLongMPIType;
      break;
    case exaULong_t:
      out=exaLongMPIType;
      break;
    case exaScalar_t:
      out=exaScalarMPIType;
      break;
    case exaByte_t:
      out=exaByteMPIType;
      break;
    default:
      break;
  }
  return out;
}

MPI_Op exaOpGetMPIOp(exaOp t) {
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

void exaDataTypeGetMin(exaDataType t,void *out) {
  switch(t) {
    case exaInt_t:
      *((exaInt *)out)=exaIntMIN;
      break;
    case exaUInt_t:
      *((exaUInt *)out)=exaUIntMIN;
      break;
    case exaLong_t:
      *((exaLong *)out)=exaLongMIN;
      break;
    case exaULong_t:
      *((exaULong *)out)=exaULongMIN;
      break;
    case exaScalar_t:
      *((exaScalar *)out)=exaScalarMIN;
      break;
    default:
      break;
  }
}

void exaDataTypeGetMax(exaDataType t,void *out) {
  switch(t) {
    case exaInt_t:
      *((exaInt *)out)=exaIntMAX;
      break;
    case exaUInt_t:
      *((exaUInt *)out)=exaUIntMAX;
      break;
    case exaLong_t:
      *((exaLong *)out)=exaLongMAX;
      break;
    case exaULong_t:
      *((exaULong *)out)=exaULongMAX;
      break;
    case exaScalar_t:
      *((exaScalar *)out)=exaScalarMAX;
      break;
    default:
      break;
  }
}
//
// exaCrystal
//
int exaCrystalInit(exaHandle h){
  exaCommCrystalInit(exaGetComm(h));
}

int exaCrystalFinalize(exaHandle h){
  exaCommCrystalFinalize(exaGetComm(h));
}
//
// exaHandle
//
int exaHandleGetData(exaHandle h,void **data){
  *data=h->data;
  return 0;
}

int exaHandleSetData(exaHandle h,void **data){
  h->data=*data;
  return 0;
}
//
// exaSettings
//
const char *exaGetSetting(const char *settingName,exaHandle h){
  return exaSettingsGetSetting(settingName,h->settings);
}

int exaSetSetting(const char *settingName,const char *value,exaHandle h){
  return exaSettingsSetSetting(settingName,value,h->settings);
}
