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

  exaSettings s; exaSettingsInit(h,NULL,&s);

  char *backend; exaSettingsGetNative(&backend,"backend::name",s);
  if(backend==NULL || strcmp(backend,argv[1])!=0)
    fprintf(stderr,"exaSettingsGet failed: backend::name != %s\n",
      argv[1]);

  exaSettingsSet("order",getExaUInt(10),s);
  exaUInt order; exaSettingsGetNative(&order,"order",s);
  if(order!=10)
    fprintf(stderr,"exaSettingsGetSetting failed: order != 20\n");

  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
