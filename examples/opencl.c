#include "exa-opencl.h"
#include "exa.h"

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
  //exaKernelCreate(p,"square",&k,3,exaVector_t,exaVector_t,exaUInt_t);
  exaKernelCreate(p,"square",&k,1,exaVector_t);

  //exaKernelRun(k,input,output,10);
  exaKernelRun(k,input);

  exaScalar in[10];
  exaVectorRead(input,(void**)&in);
  for(int i=0;i<10;i++) printf("i: %d i^2:%lf\n",i,in[i]);

  exaKernelFree(k);
  exaProgramFree(p);
  exaVectorFree(input);
  exaVectorFree(output);
  exaFinalize(h);

  MPI_Finalize();
}
