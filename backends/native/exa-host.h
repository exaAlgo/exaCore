#ifndef _EXA_HOST_H_
#define _EXA_HOST_H_

#include "exa-impl.h"
//
// Meta info
//
const char *exaHostGetExt();
//
// Init and finalize the backend
//
int exaHostInit(exaHandle h,const char *backend);
int exaHostFinalize(exaHandle h);
//
// Update settings
//
int exaHostUpdateSettings(exaSettings s);
//
// Create an Host vector
//
int exaHostVectorCreate(exaVector x,exaUInt size);
int exaHostVectorFree(exaVector x);
int exaHostVectorGetDevicePointer(exaVector x,void **ptr,
  size_t *size);
int exaHostVectorRead (exaVector x,void *out);
int exaHostVectorWrite(exaVector x,void *in);
//
// Create an Host program
//
int exaHostProgramCreate(exaProgram p,const char *fname,
  exaSettings settings);
int exaHostProgramFree(exaProgram p);
//
// Create an Host kernel
//
int exaHostKernelCreate(exaProgram p,const char *kernelName,
  exaKernel k);
int exaHostKernelRun(exaKernel k,const int nArgs,va_list args);
int exaHostKernelFree(exaKernel k);
//
// Barrier
//
int exaHostBarrier(exaHandle h);
#endif
