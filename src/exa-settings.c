#include <exa-impl.h>
#include <exa-memory.h>

int exaSettingsInit(exaSettings *settings){
  exaMalloc(1,settings);
  exaArrayInit(&(*settings)->settings,sizeof(struct exaSetting_private),10);
  return 0;
}

int exaSettingsFree(exaSettings settings){
  exaArrayFree(settings->settings);
  exaFree(settings);
  return 0;
}

const char *exaSettingsGetSetting(const char *settingName,exaSettings s){
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++) if(strcmp(ptr[i].key,settingName)==0) break;
  if(i<size) return ptr[i].value;
  else return NULL;
}

int exaSettingsSetSetting(const char *settingName,const char *value,exaSettings s){
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++) if(strcmp(ptr[i].key,settingName)==0) break;
  if(i<size) strcpy(ptr[i].value,value);
  else{
    struct exaSetting_private t;
    strcpy(t.key,settingName);
    strcpy(t.value,value);
    exaArrayAppend(s->settings,&t);
  }
}
