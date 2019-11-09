#include "exa-opencl.h"
#include "exa.h"

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,"/opencl/gpu");

  exaVector vec;
  exaVectorCreate(h,10,&vec);

  exaProgram p;
  exaProgramCreate(h,"kernels.cl",&p);

  exaProgramFree(p);
  exaVectorFree(vec);
  exaFinalize(h);

  MPI_Finalize();
}
