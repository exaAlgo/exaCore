#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaSettings s;
  exaSettingsInit(&s);
  exaSettingsSetSetting("backend",argv[1],s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaProgram p;
  exaProgramCreate(h,"kernels",s,&p);

  exaKernel k;
  exaKernelCreate(p,"vectorAdd",&k);

  exaDestroy(k);
  exaDestroy(p);
  exaFinalize(h);

  exaDestroy(s);

  MPI_Finalize();

  return 0;
}
