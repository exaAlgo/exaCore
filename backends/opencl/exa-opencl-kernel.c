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

static cl_ulong kernelTime=0;
int exaOpenCLKernelRun(exaKernel k,exaDim dim,exaKernelArg args){
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

  //TODO: This should be part of tuning phase
  //oclk->global=nThreads;
  //err=clGetKernelWorkGroupInfo(oclk->kernel,oclh->deviceId,CL_KERNEL_WORK_GROUP_SIZE,
  //  sizeof(oclk->local),&oclk->local,NULL);
  //exaOpenCLChk(err);

  //size_t multiple=oclk->global/oclk->local;
  //size_t remainder=oclk->global-multiple*oclk->local;
  //if(remainder) oclk->global=(multiple+1)*oclk->local;

  for(int i=0; i<1000; i++){
    cl_event event;
    err=clEnqueueNDRangeKernel(oclh->queue,oclk->kernel,dim->dim,NULL,
      (const size_t*)dim->global,(const size_t*)dim->local,0,NULL,&event);

    //TODO: Provide an API for the user to collect statistics.
    // This is a temporary fix.
    clWaitForEvents(1,&event);
    clFinish(oclh->queue);

    cl_ulong time_start;
    cl_ulong time_end;
    clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_START,sizeof(time_start),
      &time_start, NULL);
    clGetEventProfilingInfo(event,CL_PROFILING_COMMAND_END,sizeof(time_end),
      &time_end, NULL);
    kernelTime+=(time_end-time_start);
  }

  printf("Kernel time: %lf ms.\n",kernelTime/(1.0*1e6));

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
