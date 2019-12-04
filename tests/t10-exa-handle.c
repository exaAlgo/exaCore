#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: ./exa-init-test <backend>\n");
    exit(0);
  }

  exaSettings s;
  exaSettingsInit(&s);
  exaSettingsSetSetting("backend",argv[1],s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
