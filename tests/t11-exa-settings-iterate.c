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

  const char *key=exaSettingsIterateKeys("backend::nam",s);
  char *backend; exaSettingsGet(&backend,key,s);
  if(backend==NULL || strcmp(backend,argv[1])!=0)
    fprintf(stderr,"key: backend::name, value %s != %s",
      backend,argv[1]);

  int count=0;
  const char *prefix="defines::";
  key=exaSettingsIterateKeys(prefix,s);
  while(key!=NULL){
    if(strncmp(key,prefix,strlen(prefix))!=0)
      fprintf(stderr,"key %s does not begin with %s",key,prefix);
    count++;
    key=exaSettingsIterateKeys(NULL,s);
  }

  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
