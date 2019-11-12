#include "exa-opencl.h"
#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,"/opencl/gpu");

  exaVector input,output;
  exaVectorCreate(h,10,&input);
  exaVectorCreate(h,10,&output);

  exaProgram p;
  exaProgramCreate(h,"kernels.cl",&p);

  exaKernel k;
  exaKernelCreate(p,"square",&k,1,exaVector_t);

  //exaKernelRun(k,input,output,10);
  exaKernelRun(k,input);
  exaBarrier(h);

  exaScalar *in; exaCalloc(10,&in);
  exaVectorRead(input,in);
  for(int i=0;i<10;i++) printf("%d: %lf\n",i,in[i]);
  exaFree(in);

  exaKernelFree(k);
  exaProgramFree(p);
  exaVectorFree(input);
  exaVectorFree(output);
  exaFinalize(h);

  MPI_Finalize();
}
