#include "exa-occa-impl.h"

static const char *exaOccaExt="okl";
const char *exaOccaGetExt(){ return exaOccaExt; }

static const char *occaSerial="mode: 'Serial'";
static const char *occaCUDA  ="mode      : 'CUDA',"
                              "device_id : %d";
static const char *occaOpenCL="mode        : 'OpenCL',"
                              "platform_id : %d,"
                              "device_id   : %d";
static const char *occaOpenMP="mode     : 'OpenMP',"
                              "schedule : '%s',"
                              "chunk    : %d";

int exaOccaInit(exaHandle h,const char *backend)
{
  if(exaRank(h)==0)
    exaDebug(h,"[exaOccaInit]\n");

  exaOccaHandle oh;
  exaMalloc(1,&oh);

  char *in,config[5][BUFSIZ];
  exaCalloc(strlen(backend)+1,&in);
  strcpy(in,backend);

  int nArgs=0;
  char *pch=strtok(in,"/");
  while(pch!=NULL){
    strcpy(config[nArgs++],pch);
    pch=strtok(NULL,"/");
  }
  exaFree(in);

  assert(strcmp("occa",config[0])==0);

  char deviceConfig[BUFSIZ];
  if(strcmp(config[1],"cpu")==0)
    strcpy(deviceConfig,occaSerial);
  else if(strcmp(config[1],"gpu")==0){
    int deviceId=0;
    int platformId=0;
    if(nArgs>3) deviceId=atoi(config[3]);
    if(nArgs>4) platformId=atoi(config[4]);

    if(strcmp(config[2],"cuda")==0){
      snprintf(deviceConfig,BUFSIZ,occaCUDA,deviceId);
    } else if(strcmp(config[2],"opencl")==0){
      snprintf(deviceConfig,BUFSIZ,occaOpenCL,platformId,deviceId);
    }
  }else if(strcmp(config[1],"openmp")==0){
    char schedule[BUFSIZ]="compact";
    int chunkSize=8;

    if(nArgs>2) strcpy(schedule,config[2]);
    if(nArgs>3) chunkSize=atoi(config[3]);

    snprintf(deviceConfig,BUFSIZ,occaOpenMP,schedule,chunkSize);
  }

  if(exaRank(h)==0) exaDebug(h,"deviceConfig=\"%s\"\n",deviceConfig);
  oh->device=occaCreateDeviceFromString(deviceConfig);

  exaHandleSetData(h,(void **)&oh);

  if(exaRank(h)==0) exaDebug(h,"Set backend handles ...\n");
  // set call back functions for the backend
  h->backendFinalize=exaOccaFinalize;
  h->updateSettings=exaOccaUpdateSettings;
  h->backendExt=exaOccaGetExt;
  h->vectorCreate=exaOccaVectorCreate;
  h->vectorFree=exaOccaVectorFree;
  h->programCreate=exaOccaProgramCreate;
  h->programFree=exaOccaProgramFree;
  h->kernelCreate=exaOccaKernelCreate;
  h->kernelFree=exaOccaKernelFree;
  h->barrier=exaOccaBarrier;

  if(exaRank(h)==0)
    exaDebug(h,"[/exaOccaInit]\n");

  return 0;
}

int exaOccaUpdateSettings(exaSettings s){
  exaSettingsSet("defines::p_blockSize",getExaUInt(256),s);
  return 0;
}

int exaOccaBarrier(exaHandle h){
  exaOccaHandle oh;
  exaHandleGetData(h,(void**)&oh);
  occaDeviceFinish(oh->device);

  return 0;
}

int exaOccaFinalize(exaHandle h){
  exaOccaHandle oh;
  exaHandleGetData(h,(void **)&oh);

  occaFree((occaType*)&oh->device);

  exaFree(oh);
  oh=NULL;
  exaHandleSetData(h,(void **)&oh);

  return 0;
}

__attribute__((constructor))
static void Register(void){
  exaRegister(exaOccaInit,"/occa/gpu/opencl",10);
  exaRegister(exaOccaInit,"/occa/gpu/cuda",10);
  exaRegister(exaOccaInit,"/occa/cpu",10);
  exaRegister(exaOccaInit,"/occa/openmp",10);
}

occaType exaValueToOccaType(exaValue v){
  occaType t;
  switch(v->type){
    case exaInt_t:
      t=occaInt(v->value.i);
      break;
    case exaUInt_t:
      t=occaUInt(v->value.ui);
      break;
    case exaLong_t:
      t=occaLong(v->value.l);
      break;
    case exaULong_t:
      t=occaULong(v->value.ul);
      break;
    case exaScalar_t:
      if(strcmp(exaScalarString,"double")==0)
        t=occaDouble(v->value.s);
      else
        t=occaFloat(v->value.s);
      break;
    case exaChar_t:
      t=occaChar(v->value.b);
      break;
    case exaStr_t:
      t=occaString(v->value.str);
      break;
    default:
      break;
  }

  return t;
}
