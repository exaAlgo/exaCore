#include "exa-opencl-impl.h"
#include "exa-memory.h"

int exaOpenCLInit(exaHandle h,const char *backend){
  exaOpenCLHandle oclh;
  exaMalloc(1,&oclh);

  // set platformId,deviceType
  // these should be set based on user input
  oclh->platformId=0;
  oclh->deviceType=CL_DEVICE_TYPE_GPU;

  cl_int err;
  err=clGetDeviceIDs(oclh->platformId,oclh->deviceType,1,&oclh->deviceId,NULL);
  exaOpenCLChk(err);

  oclh->context=clCreateContext(NULL,1,&oclh->deviceId,NULL,NULL,&err);
  exaOpenCLChk(err);

  oclh->queue=clCreateCommandQueue(oclh->context,oclh->deviceId,NULL,&err);
  exaOpenCLChk(err);

  exaHandleSetData(h,(void **)&oclh);

  // set call back functions for the backend
  h->backendFinalize=exaOpenCLFinalize;
  h->vectorCreate=exaOpenCLVectorCreate;
}

int exaOpenCLFinalize(exaHandle h){
  exaOpenCLHandle oclh; exaHandleGetData(h,(void **)&oclh);
  clReleaseCommandQueue(oclh->queue);
  clReleaseContext(oclh->context);
  exaFree(oclh);
  oclh=NULL;
  exaHandleSetData(h,(void **)&oclh);
  return 0;
}

int exaOpenCLvectorCreate(exaVector x,exaInt size){
}
