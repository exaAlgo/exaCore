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
  h->vectorFree=exaOpenCLVectorFree;
  h->programCreate=exaOpenCLProgramCreate;
  h->programFree=exaOpenCLProgramFree;
  h->kernelCreate=exaOpenCLKernelCreate;
  h->kernelFree=exaOpenCLKernelFree;
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
//
// Create an Opencl vector
//
int exaOpenCLVectorCreate(exaVector x,exaInt size){
  exaHandle h;
  exaOpenCLHandle oclh;
  exaVectorGetHandle(x,&h);
  exaHandleGetData(h,(void**)&oclh);

  x->getDevicePointer=exaOpenCLVectorGetDevicePointer;

  exaOpenCLVector vec;
  exaMalloc(1,&vec);
  cl_int err;
  vec->data=clCreateBuffer(oclh->context,CL_MEM_READ_WRITE,
    sizeof(double)*size,NULL,&err);
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

int exaOpenCLVectorFree(exaVector x){
  exaOpenCLVector vec;
  exaVectorGetData(x,(void**)&vec);

  clReleaseMemObject(vec->data);

  exaFree(vec);
  vec=NULL;
  exaVectorSetData(x,(void **)&vec);

  return 0;
}
//
// Create an OpenCL program
//
int exaOpenCLProgramCreate(exaProgram p,const char *fname){
  exaHandle h;
  exaOpenCLHandle oclh;
  exaProgramGetHandle(p,&h);
  exaHandleGetData(h,(void**)&oclh);

  exaLong size;
  char *source;
  FILE *fp;

  int rank=exaRank(h);
  if(rank==0){
    fp=fopen(fname,"rb");
    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    fseek(fp,0,SEEK_SET);
  }

  exaBcast(h,&size,1,exaLong_t);
  exaMalloc(size*sizeof(char)+1,&source);

  size_t read;
  if(rank==0){
    read=fread(source,sizeof(char),size,fp);
    assert(read==size);
  }
  exaBcast(h,source,size*sizeof(char),exaByte_t);

  exaOpenCLProgram oclp;
  exaMalloc(1,&oclp);

  cl_int err;
  oclp->program=clCreateProgramWithSource(oclh->context,1,
    (const char**)&source,NULL,&err);
  exaOpenCLChk(err);

  exaFree(source);

  err=clBuildProgram(oclp->program,1,&oclh->deviceId,NULL,NULL,NULL);
  exaOpenCLChk(err);
#if 0
  size_t len;
  char buffer[2048];
  clGetProgramBuildInfo(oclp->program,oclh->deviceId,CL_PROGRAM_BUILD_LOG,
    sizeof(buffer),buffer,&len);
  printf("%s\n", buffer);
#endif

  exaProgramSetData(p,(void**)&oclp);
}

int exaOpenCLProgramFree(exaProgram p){
  exaOpenCLProgram oclp;
  exaProgramGetData(p,(void**)&oclp);

  clReleaseProgram(oclp->program);

  exaFree(oclp);
  oclp=NULL;
  exaProgramSetData(p,(void**)&oclp);

  return 0;
}
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

int exaOpenCLKernelRun(exaKernel k,exaKernelArg args){
  exaHandle h;
  exaKernelGetHandle(k,&h);
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);

  exaOpenCLKernel oclk;
  exaKernelGetData(k,(void**)&oclk);

  cl_int err;
  for(int i=0;i<k->nArgs;k++){
    err=clSetKernelArg(oclk->kernel,i,args[i].size,args[i].arg);
    exaOpenCLChk(err);
  }

  err=clGetKernelWorkGroupInfo(oclk->kernel,oclh->deviceId,CL_KERNEL_WORK_GROUP_SIZE,
    sizeof(oclk->local),&oclk->local,NULL);
  exaOpenCLChk(err);
  oclk->global=10;

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

__attribute__((constructor))
static void Register(void){
  exaRegister(exaOpenCLInit,"/opencl/gpu");
  exaRegister(exaOpenCLInit,"/opencl/cpu");
}
