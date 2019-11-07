#include "exa-opencl.h"
#include "exa.h"

int main(int argc,char *argv){
  MPI_Init(NULL,NULL);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD);

  exaOpenCLInit(h);

  exaFinalize(h);
  MPI_Finalize();
}
