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

int exaOpenCLKernelRun(exaKernel k,const int nArgs,...){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);

  exaOpenCLKernel oclk;
  exaKernelGetData(k,(void**)&oclk);

#if 0
  cl_int err;
  for(int i=0;i<nArgs;i++){
    err=clSetKernelArg(oclk->kernel,i,args[i].size,args[i].arg);
    exaOpenCLChk(err);
  }

  cl_event event;
  err=clEnqueueNDRangeKernel(oclh->queue,oclk->kernel,dim->dim,NULL,
    (const size_t*)dim->global,(const size_t*)dim->local,0,NULL,&event);

  clWaitForEvents(1,&event);
  clFinish(oclh->queue);

  exaOpenCLChk(err);
#endif

  return 0;
}

int exaOpenCLKernelFree(exaKernel k)
{
  exaOpenCLKernel oclk;
  exaKernelGetData(k,(void**)&oclk);

  clReleaseKernel(oclk->kernel);

  exaFree(oclk);
  oclk=NULL;
  exaKernelSetData(k,(void**)&oclk);

  return 0;
}
