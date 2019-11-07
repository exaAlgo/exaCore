#include "exa-opencl-impl.h"
#include "exa-memory.h"

void exaOpenCLChk(cl_int err){
  switch(err){
    case CL_INVALID_PLATFORM:
      fprintf(stderr,"CL_INVALID_PLATFORM\n");
      break;
    case CL_INVALID_DEVICE_TYPE:
      fprintf(stderr,"CL_INVALID_DEVICE_TYPE\n");
      break;
    case CL_INVALID_VALUE:
      fprintf(stderr,"CL_INVALID_VALUE\n");
      break;
    case CL_DEVICE_NOT_FOUND:
      fprintf(stderr,"CL_DEVICE_NOT_FOUND\n");
      break;
    default:
      break;
  }
}
int exaInitOpenCL(exaHandle h){
  exaOpenCLHandle oclh;
  exaMalloc(1,&oclh);

  // set platformId,deviceType,deviceId
  // these should be set based on user input
  oclh->platformId=0;
  oclh->deviceId=0;
  oclh->deviceType=CL_DEVICE_TYPE_GPU;

  cl_int err;
  oclh->context=clCreateContext(NULL,1,&oclh->deviceId,NULL,NULL,&err);
  exaOpenCLChk(err);

  oclh->queue=clCreateCommandQueue(oclh->context,oclh->deviceId,NULL,&err);
  exaOpenCLChk(err);
}

int vectorCreateOpenCL(exaVector x,exaInt size){
}
