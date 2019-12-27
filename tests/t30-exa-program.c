#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaSettings s; exaSettingsCreate(h,NULL,&s);

  exaProgram p;
  exaProgramCreate(h,argv[0],s,&p);

  exaDestroy(p);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
