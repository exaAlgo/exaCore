#include "exa-opencl-impl.h"
#include "exa-memory.h"
//
// Create an OpenCL program
//
int exaOpenCLProgramCreate(exaProgram p,const char *fname,exaSettings kernelSettings){
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

  exaProgramSetData(p,(void**)&oclp);

  return 0;
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
