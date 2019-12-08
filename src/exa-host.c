#include "exa-host.h"

static const char *exaHostExt;
const char *exaHostGetExt(){ return exaHostExt; }
//
// Init and finalize the backend: dummy
//
int exaHostInit(exaHandle h,const char *backend)
{
  // set call back functions for the backend
  h->backendFinalize=exaHostFinalize;
  h->backendExt=exaHostGetExt;
  h->vectorCreate=exaHostVectorCreate;
  h->vectorFree=exaHostVectorFree;
  h->programCreate=exaHostProgramCreate;
  h->programFree=exaHostProgramFree;
  h->kernelCreate=exaHostKernelCreate;
  h->kernelFree=exaHostKernelFree;
  h->barrier=exaHostBarrier;

  return 0;
}
int exaHostFinalize(exaHandle h){ return 0; }
//
// Create an Host program
//
int exaHostProgramCreate(exaProgram p,const char *fname,
  exaSettings settings){ return 0; }
int exaHostProgramFree(exaProgram p){ return 0; }
//
// Create an Host kernel
//
int exaHostKernelCreate(exaProgram p,const char *kernelName,
  exaKernel k)
{
  k->runKernel=exaHostKernelRun;
  return 0;
}
int exaHostKernelRun(exaKernel k,const int nArgs,va_list args){ return 0; }
int exaHostKernelFree(exaKernel k){ return 0; }
//
// Barrier
//
int exaHostBarrier(exaHandle h){ return 0; }

__attribute__((constructor))
static void Register(void){
  exaRegister(exaHostInit,"/host",10);
}
