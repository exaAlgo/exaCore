#include "exa.h"
#include "exa-memory.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

void mxm(exaInt M,exaScalar *inA,exaScalar *inB,exaScalar *out){
  for(int i=0;i<M;i++)
    for(int k=0;k<M;k++){
      out[i*M+k]=0.0;
      for(int j=0;j<M;j++)
        out[i*M+k]+=inA[i*M+j]*inB[j*M+k];
    }
}

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaSettings s; exaSettingsInit(&s);
  exaSettingsSetSetting("backend","/opencl/gpu",s);
  exaSettingsSetSetting("debug","on",s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaUInt M,N,K; M=N=K=128;
  exaVector inputA,inputB,output;
  exaVectorCreate(h,M*N,&inputA);
  exaVectorCreate(h,N*K,&inputB);
  exaVectorCreate(h,M*K,&output);

  exaProgram p;
  exaProgramCreate(h,"kernels",s,&p);

#if 0
  exaKernel k;
  exaKernelCreate(p,"mxm",&k,6,exaUInt_t,exaUInt_t,exaUInt_t,
    exaVector_t,exaVector_t,exaVector_t);

  srand(time(0));
  exaScalar *inA; exaCalloc(M*N,&inA);
  exaScalar *inB; exaCalloc(N*K,&inB);
  exaInt count=0;
  for(int i=0;i<M;i++)
    for(int j=0;j<N;j++)
      inA[count++]=rand()%10;

  count=0;
  for(int i=0;i<N;i++)
    for(int j=0;j<K;j++)
      inB[count++]=rand()%10;

  exaVectorWrite(inputA,inA);
  exaVectorWrite(inputB,inB);

  size_t global[2]={M,K};
  size_t local [2]={16,16};
  exaDim dim; exaDimInit(&dim,2,global,local);
  exaKernelRun(k,dim,M,N,K,inputA,inputB,output);
  exaDimFree(dim);
  exaBarrier(h);

  exaScalar *out; exaCalloc(M*K,&out);
  exaVectorRead(output,out);
  exaScalar *outHost; exaCalloc(M*K,&outHost);
  mxm(M,inA,inB,outHost);
  for(int i=0;i<M*K;i++)
    if(fabs(out[i]-outHost[i])>1e-8){
      printf("mxm failed: %lf %lf.\n",out[i],outHost[i]);
      exit(1);
    }
  printf("mxm passed.\n");

  exaFree(inA);
  exaFree(inB);
  exaFree(out);
  exaFree(outHost);

  exaSettingsFree(s);
  exaKernelFree(k);
#endif
  exaProgramFree(p);
  exaFinalize(h);

  MPI_Finalize();
}
