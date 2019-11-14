#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaSettings s; exaSettingsInit(&s);
  exaSettingsSetSetting("backend","/opencl/gpu",s);
  exaSettingsSetSetting("debug","on",s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaUInt M,N,K; M=N=K=3;
  exaVector inputA,inputB,output;
  exaVectorCreate(h,M*N,&inputA);
  exaVectorCreate(h,N*K,&inputB);
  exaVectorCreate(h,M*K,&output);

  exaProgram p;
  exaProgramCreate(h,"kernels.cl",&p);

  exaKernel k;
  exaKernelCreate(p,"mxm",&k,6,exaUInt_t,exaUInt_t,exaUInt_t,
    exaVector_t,exaVector_t,exaVector_t);

  exaScalar *inA; exaCalloc(M*N,&inA);
  exaScalar *inB; exaCalloc(N*K,&inB);
  exaInt count=0;
  for(int i=0;i<M;i++)
    for(int j=0;j<N;j++)
      inA[count++]=i+j;

  count=0;
  for(int i=0;i<N;i++)
    for(int j=0;j<K;j++)
      inB[count++]=1.0;

  exaVectorWrite(inputA,inA);
  exaVectorWrite(inputB,inB);
  exaFree(inA);
  exaFree(inB);

  size_t global[2]={M,K};
  size_t local [2]={1,1};
  exaDim dim; exaDimInit(&dim,2,global,local);
  exaKernelRun(k,dim,M,N,K,inputA,inputB,output);
  exaDimFree(dim);
  exaBarrier(h);

  exaScalar *out; exaCalloc(M*K,&out);
  exaVectorRead(output,out);
  for(int i=0;i<M*K;i++) printf("%d: %lf\n",i,out[i]);
  exaFree(out);

  exaSettingsFree(s);
  exaKernelFree(k);
  exaProgramFree(p);
  exaFinalize(h);

  MPI_Finalize();
}
