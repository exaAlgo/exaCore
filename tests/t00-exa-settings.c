#include "exa.h"
#include "exa-memory.h"

int main(int argc,char *argv[])
{
  exaSettings s;
  exaSettingsInit(&s);
  exaSettingsSetSetting("backend","/occa/cpu",s);
  exaSettingsSetSetting("debug","on",s);

  const char *backend=exaSettingsGetSetting("backend",s);
  if(strcmp(backend,"/occa/cpu")!=0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s != %s\n",backend,"/occa/cpu");
  if(strcmp(backend,"/occa/gpu")==0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s == %s\n",backend,"/occa/cpu");

  const char *debug=exaSettingsGetSetting("debug",s);
  if(strcmp(debug,"on")!=0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s != on\n",debug);

  exaSettingsSetSetting("debug","off",s);
  debug=exaSettingsGetSetting("debug",s);
  if(strcmp(debug,"off")!=0)
    fprintf(stderr,"exaSettingsGetSetting failed: %s != off\n",debug);

  exaDestroy(s);
}
