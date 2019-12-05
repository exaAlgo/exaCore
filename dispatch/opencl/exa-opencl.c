#include "exa-opencl-impl.h"

static const char *exaOpenCLExt="cl";
const char *exaOpenCLGetExt(){ return exaOpenCLExt; }

int exaOpenCLInit(exaHandle h,const char *backend){
  exaOpenCLHandle oclh;
  exaMalloc(1,&oclh);

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

  oclh->deviceType=CL_DEVICE_TYPE_GPU;
  if(strcmp(config[1],"cpu")==0)
    oclh->deviceType=CL_DEVICE_TYPE_CPU;

  int platformId,deviceId;
  platformId=deviceId=0;
  if(nArgs>2) platformId=atoi(config[2]);
  if(nArgs>3) platformId=atoi(config[3]);

  cl_uint platformCount;
  clGetPlatformIDs(0,NULL,&platformCount);

  cl_platform_id *platforms;
  exaMalloc(platformCount,&platforms);
  clGetPlatformIDs(platformCount,platforms,NULL);
  oclh->platformId=platforms[platformId];
  exaFree(platforms);

  cl_uint deviceCount;
  clGetDeviceIDs(oclh->platformId,oclh->deviceType,0,NULL,
    &deviceCount);

  cl_device_id *devices;
  exaMalloc(deviceCount,&devices);
  clGetDeviceIDs(oclh->platformId,oclh->deviceType,deviceCount,
    devices,NULL);
  oclh->deviceId=devices[deviceId];
  exaFree(devices);

  cl_int err;
  oclh->context=clCreateContext(0,1,&oclh->deviceId,NULL,NULL,&err);
  exaOpenCLChk(err);

  const char *debug=exaGetSetting("debug",h);
  cl_command_queue_properties prop[]={CL_QUEUE_PROPERTIES,
    CL_QUEUE_PROFILING_ENABLE,0};
  if(strcmp(debug,"on")==0)
    oclh->queue=clCreateCommandQueueWithProperties(oclh->context,
      oclh->deviceId,prop,&err);
  else
    oclh->queue=clCreateCommandQueueWithProperties(oclh->context,
      oclh->deviceId,NULL,&err);
  exaOpenCLChk(err);

  exaHandleSetData(h,(void **)&oclh);

  // set call back functions for the backend
  h->backendFinalize=exaOpenCLFinalize;
  h->backendExt=exaOpenCLGetExt;
  h->vectorCreate=exaOpenCLVectorCreate;
  h->vectorFree=exaOpenCLVectorFree;
  h->programCreate=exaOpenCLProgramCreate;
  h->programFree=exaOpenCLProgramFree;
  h->kernelCreate=exaOpenCLKernelCreate;
  h->kernelFree=exaOpenCLKernelFree;
  h->barrier=exaOpenCLBarrier;
}

int exaOpenCLBarrier(exaHandle h){
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);

  clFinish(oclh->queue);

  return 0;
}

int exaOpenCLFinalize(exaHandle h){
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void **)&oclh);

  clReleaseCommandQueue(oclh->queue);
  clReleaseContext(oclh->context);

  exaFree(oclh);
  oclh=NULL;
  exaHandleSetData(h,(void **)&oclh);

  return 0;
}

__attribute__((constructor))
static void Register(void){
  exaRegister(exaOpenCLInit,"/opencl/gpu",10);
  exaRegister(exaOpenCLInit,"/opencl/cpu",10);
}
