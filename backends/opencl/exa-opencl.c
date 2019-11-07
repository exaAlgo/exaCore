#include "exa-opencl-impl.h"
#include "exa-memory.h"

int exaOpenCLInit(exaHandle h){
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
}

int exaOpenCLFinalize(exaHandle h){}

int vectorCreateOpenCL(exaVector x,exaInt size){
}
