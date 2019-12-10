#include "exa-opencl-impl.h"
#include "exa-memory.h"
//
// Create an OpenCL program
//
int exaOpenCLProgramCreate(exaProgram p,const char *fname,
  exaSettings kernelSettings)
{
  exaHandle h;
  exaOpenCLHandle oclh;
  exaProgramGetHandle(p,&h);
  exaHandleGetData(h,(void**)&oclh);

  exaOpenCLProgram oclp;
  exaCalloc(1,&oclp);

  const char *ext=exaOpenCLGetExt();
  exaMalloc(strlen(fname)+1+strlen(ext),&oclp->fileName);
  strcpy(oclp->fileName                ,fname);
  strcpy(oclp->fileName+strlen(fname)  ,"."  );
  strcpy(oclp->fileName+strlen(fname)+1,ext  );
  exaDebug(h,"Source file name=%s\n",oclp->fileName);

  exaLong size;
  FILE *fp;

  int rank=exaRank(h);
  if(rank==0){
    fp=fopen(oclp->fileName,"r");
    fseek(fp,0,SEEK_END);
    size=ftell(fp);
    fseek(fp,0,SEEK_SET);
  }

  exaBcast(h,&size,1,exaLong_t);
  exaCalloc((size+1)*sizeof(char),&oclp->source);

  size_t read;
  if(rank==0){
    read=fread(oclp->source,sizeof(char),size,fp);
    fclose(fp);
    assert(read==size);
  }
  exaBcast(h,oclp->source,(size+1)*sizeof(char),exaByte_t);

  cl_int err;
  oclp->program=clCreateProgramWithSource(oclh->context,1,
    (const char**)&oclp->source,NULL,&err);
  exaOpenCLChk(err);

  err=clBuildProgram(oclp->program,0,NULL,NULL,NULL,NULL);
  exaOpenCLChk(err);

  exaProgramSetData(p,(void**)&oclp);

  return 0;
}

int exaOpenCLProgramFree(exaProgram p){
  exaOpenCLProgram oclp;
  exaProgramGetData(p,(void**)&oclp);

  clReleaseProgram(oclp->program);
  exaFree(oclp->fileName);
  exaFree(oclp->source);

  exaFree(oclp);
  oclp=NULL;
  exaProgramSetData(p,(void**)&oclp);

  return 0;
}
