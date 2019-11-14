#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaSettings s; exaSettingsInit(&s);
  exaSettingsSetSetting("backend","/opencl/gpu",s);
  exaSettingsSetSetting("debug","on",s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaUInt M=10;
  exaVector inputA,inputB,output;
  exaVectorCreate(h,M,&inputA);
  exaVectorCreate(h,M,&inputB);
  exaVectorCreate(h,M,&output);

  exaProgram p;
  exaProgramCreate(h,"kernels.cl",&p);

  exaKernel k;
  exaKernelCreate(p,"vectorAdd",&k,4,exaVector_t,exaVector_t,
    exaVector_t,exaUInt_t);

  exaScalar *inA; exaCalloc(M,&inA);
  exaScalar *inB; exaCalloc(M,&inB);
  for(int i=0;i<M;i++)
    inA[i]=i+0.1;

  for(int i=0;i<M;i++)
    inB[i]=5.0-i-0.1;

  exaVectorWrite(inputA,inA);
  exaVectorWrite(inputB,inB);
  exaFree(inA);
  exaFree(inB);

  size_t global[1]={M};
  size_t local [1]={1};
  exaDim dim; exaDimInit(&dim,1,global,local);
  exaKernelRun(k,dim,inputA,inputB,output,M);

  exaBarrier(h);

  exaScalar *out; exaCalloc(M,&out);
  exaVectorRead(output,out);
  for(int i=0;i<M;i++) printf("%d: %lf\n",i,out[i]);
  exaFree(out);

  exaDimFree(dim);
  exaSettingsFree(s);
  exaKernelFree(k);
  exaProgramFree(p);
  exaFinalize(h);

  MPI_Finalize();
}
