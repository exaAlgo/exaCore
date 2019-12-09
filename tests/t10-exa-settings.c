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

  const char *backend=exaSettingsGetSetting("backend:name",s);
  if(strcmp(backend,argv[1])!=0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s != %s\n",backend,argv[1]);
  if(strcmp(backend,"/invalid/backend")==0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s == %s\n",backend,"/occa/cpu");

  exaSettingsSetSetting("debug","off",s);
  const char *debug=exaSettingsGetSetting("debug",s);
  if(strcmp(debug,"off")!=0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s != off\n",debug);

  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
