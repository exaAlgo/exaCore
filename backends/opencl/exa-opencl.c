#include "exa-opencl-impl.h"
#include "exa-memory.h"

int exaOpenCLInit(exaHandle h,const char *backend){
  exaOpenCLHandle oclh;
  exaMalloc(1,&oclh);

  char *in,config[4][BUFSIZ];
  exaMalloc(strlen(backend)+1,&in);
  strcpy(in,backend);

  int i=0;
  char *pch=strtok(in,"/");
  while(pch!=NULL){
    strcpy(config[i],pch);
    //printf("i=%d config[%d]=%s\n",i,i,config[i]);
    i++;
    pch=strtok(NULL,"/");
  }
  exaFree(in);

  oclh->deviceType=CL_DEVICE_TYPE_GPU;
  if(strcmp(config[1],"cpu")==0)
    oclh->deviceType=CL_DEVICE_TYPE_CPU;

  int platformId;//=atoi(config[2]);
  int deviceId;//=atoi(config[3]);
  platformId=deviceId=0;

  cl_uint platformCount;
  clGetPlatformIDs(0,NULL,&platformCount);

  cl_platform_id *platforms;
  exaMalloc(platformCount,&platforms);
  clGetPlatformIDs(platformCount,platforms,NULL);
  oclh->platformId=platforms[platformId];
  exaFree(platforms);

  cl_uint deviceCount;
  clGetDeviceIDs(oclh->platformId,oclh->deviceType,0,NULL,&deviceCount);

  cl_device_id *devices;
  exaMalloc(deviceCount,&devices);
  clGetDeviceIDs(oclh->platformId,oclh->deviceType,deviceCount,devices,NULL);
  oclh->deviceId=devices[deviceId];
  exaFree(devices);

  cl_int err;
  oclh->context=clCreateContext(0,1,&oclh->deviceId,NULL,NULL,&err);
  exaOpenCLChk(err);

  oclh->queue=clCreateCommandQueueWithProperties(oclh->context,oclh->deviceId,NULL,&err);
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
  h->barrier=exaOpenCLBarrier;
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
    fp=fopen(fname,"r");
    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    fseek(fp,0,SEEK_SET);
  }

  exaBcast(h,&size,1,exaLong_t);
  exaCalloc((size+1)*sizeof(char),&source);

  size_t read;
  if(rank==0){
    read=fread(source,sizeof(char),size,fp);
    assert(read==size);
  }
  exaBcast(h,source,(size+1)*sizeof(char),exaByte_t);

  exaOpenCLProgram oclp;
  exaMalloc(1,&oclp);

  cl_int err;
  oclp->program=clCreateProgramWithSource(oclh->context,1,(const char**)&source,NULL,&err);
  exaOpenCLChk(err);

  exaFree(source);

  err=clBuildProgram(oclp->program,0,NULL,NULL,NULL,NULL);
  exaOpenCLChk(err);
#if 0
  if(err!=CL_SUCCESS){
  size_t len;
  char buffer[2048];
  clGetProgramBuildInfo(oclp->program,oclh->deviceId,CL_PROGRAM_BUILD_LOG,
    sizeof(buffer),buffer,&len);
  printf("%s\n", buffer);
  }
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
  for(int i=0;i<k->nArgs;i++){
    err=clSetKernelArg(oclk->kernel,i,args[i].size,args[i].arg);
    exaOpenCLChk(err);
  }

  err=clGetKernelWorkGroupInfo(oclk->kernel,oclh->deviceId,CL_KERNEL_WORK_GROUP_SIZE,
    sizeof(oclk->local),&oclk->local,NULL);
  exaOpenCLChk(err);
  oclk->global=10;
  oclk->local=1;

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

int exaOpenCLBarrier(exaHandle h){
  exaOpenCLHandle oclh;
  exaHandleGetData(h,(void**)&oclh);

  clFinish(oclh->queue);

  return 0;
}

__attribute__((constructor))
static void Register(void){
  exaRegister(exaOpenCLInit,"/opencl/gpu");
  exaRegister(exaOpenCLInit,"/opencl/cpu");
}
