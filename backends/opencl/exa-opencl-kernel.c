#include "exa-opencl-impl.h"
#include "exa-memory.h"
//
// Create an OpenCL kernel
//
int exaOpenCLKernelCreate(exaProgram p,const char *kernelName,exaKernel k){
  exaHandle h;
  exaKernelGetHandle(k,&h);

  exaOpenCLProgram oclp;
  exaProgramGetData(p,(void**)&oclp);

  k->runKernel=exaOpenCLKernelRun;

  exaOpenCLKernel oclk;
  exaMalloc(1,&oclk);
  cl_int err;
  oclk->kernel=clCreateKernel(oclp->program,kernelName,&err);
  exaOpenCLChk(err);

  exaKernelSetData(k,(void**)&oclk);

  return 0;
}

int exaOpenCLKernelRun(exaKernel k,exaUInt nThreads,exaKernelArg args){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);

  exaOpenCLKernel oclk;
  exaKernelGetData(k,(void**)&oclk);

  cl_int err;
  for(int i=0;i<k->nArgs;i++){
    err=clSetKernelArg(oclk->kernel,i,args[i].size,args[i].arg);
    exaOpenCLChk(err);
  }

  oclk->global=nThreads;
  err=clGetKernelWorkGroupInfo(oclk->kernel,oclh->deviceId,CL_KERNEL_WORK_GROUP_SIZE,
    sizeof(oclk->local),&oclk->local,NULL);
  exaOpenCLChk(err);

  size_t multiple=oclk->global/oclk->local;
  size_t remainder=oclk->global-multiple*oclk->local;
  if(remainder) oclk->global=(multiple+1)*oclk->local;

  err=clEnqueueNDRangeKernel(oclh->queue,oclk->kernel,1,NULL,&oclk->global,
    &oclk->local,0,NULL,NULL);
  exaOpenCLChk(err);

  return 0;
}

int exaOpenCLKernelFree(exaKernel k){
  exaOpenCLKernel oclk;
  exaKernelGetData(k,(void**)&oclk);

  clReleaseKernel(oclk->kernel);

  exaFree(oclk);
  oclk=NULL;
  exaKernelSetData(k,(void**)&oclk);

  return 0;
}
