#include "exa-opencl-impl.h"
#include "exa-memory.h"
//
// Create an Opencl vector
//
int exaOpenCLVectorCreate(exaVector x,exaInt size){
  exaHandle h;
  exaOpenCLHandle oclh;
  exaVectorGetHandle(x,&h);
  exaHandleGetData(h,(void**)&oclh);

  x->getDevicePointer=exaOpenCLVectorGetDevicePointer;
  x->vectorRead=exaOpenCLVectorRead;
  x->vectorWrite=exaOpenCLVectorWrite;

  exaOpenCLVector vec;
  exaMalloc(1,&vec);
  cl_int err;
  vec->data=clCreateBuffer(oclh->context,CL_MEM_READ_WRITE,
    sizeof(exaScalar)*size,NULL,&err);
  exaOpenCLChk(err);

  exaVectorSetData(x,(void**)&vec);

  return 0;
}

int exaOpenCLVectorGetDevicePointer(exaVector x,void **ptr,size_t *size){
  exaOpenCLVector vec;
  exaVectorGetData(x,(void**)&vec);

  *ptr=&vec->data;
  *size=sizeof(cl_mem);
}

int exaOpenCLVectorRead(exaVector x,exaScalar *out){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);
  exaOpenCLVector oclv;
  exaVectorGetData(x,(void**)&oclv);

  cl_int err;
  exaInt size=exaVectorGetSize(x);
  err=clEnqueueReadBuffer(oclh->queue,oclv->data,CL_TRUE,0,sizeof(exaScalar)*size,
    out,0,NULL,NULL);
  exaOpenCLChk(err);

  return 0;
}

int exaOpenCLVectorWrite(exaVector x,exaScalar *in){
  exaHandle h;
  exaVectorGetHandle(x,&h);
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);
  exaOpenCLVector oclv;
  exaVectorGetData(x,(void**)&oclv);

  cl_int err;
  exaInt size=exaVectorGetSize(x);
  err=clEnqueueWriteBuffer(oclh->queue,oclv->data,CL_TRUE,0,sizeof(exaScalar)*size,
    in,0,NULL,NULL);
  exaOpenCLChk(err);

  return 0;
}

int exaOpenCLVectorFree(exaVector x){
  exaOpenCLVector vec;
  exaVectorGetData(x,(void**)&vec);

  clReleaseMemObject(vec->data);

  exaFree(vec);
  vec=NULL;
  exaVectorSetData(x,(void **)&vec);

  return 0;
}
