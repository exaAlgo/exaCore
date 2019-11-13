#include "exa.h"
#include "exa-memory.h"

#define N 5

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,"/opencl/gpu");

  exaVector input,output;
  exaVectorCreate(h,N,&input);
  exaVectorCreate(h,N,&output);

  exaProgram p;
  exaProgramCreate(h,"kernels.cl",&p);

  exaKernel k;
  exaKernelCreate(p,"square",&k,3,exaVector_t,exaVector_t,exaUInt_t);

  exaScalar *in; exaCalloc(N,&in);
  for(int i=0;i<N;i++) in[i]=i;
  exaVectorWrite(input,in);

  exaKernelRun(k,N,input,output,N);
  exaBarrier(h);

  exaVectorRead(output,in);
  for(int i=0;i<N;i++) printf("%d: %lf\n",i,in[i]);
  exaFree(in);

  exaKernelFree(k);
  exaProgramFree(p);
  exaVectorFree(input);
  exaVectorFree(output);
  exaFinalize(h);

  MPI_Finalize();
}
