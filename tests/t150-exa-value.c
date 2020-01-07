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

  exaValue backend=exaSettingsGetValue("backend::name",s);
  if(backend==NULL || !exaValueEq(backend,getExaStr(argv[1])))
    fprintf(stderr,"exaSettingsGet failed: backend::name != %s\n",
      argv[1]);

  exaSettingsSet("debug",getExaStr("off"),s);
  exaValue debug=exaSettingsGetValue("debug",s);
  if(debug==NULL || !exaValueEq(debug,getExaStr("off")))
    fprintf(stderr,"exaSettingsGetSetting failed: debug != off\n");

  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
