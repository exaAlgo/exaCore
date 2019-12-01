#include <exa-impl.h>
#include <exa-memory.h>
//
// exaDim: Just temporary, this should go away
//
int exaDimInit(exaDim *d_,exaUInt dim,size_t *global,size_t *local){
  exaMalloc(1,d_);
  exaDim d=*d_;
  d->dim=dim;
  memcpy(d->global,global,sizeof(size_t)*dim);
  memcpy(d->local ,local ,sizeof(size_t)*dim);

  d->info.objectType=exaDimObj;
  return 0;
}

int exaDimFree(exaDim dim){
  exaFree(dim);
  return 0;
}
//
// exaSettings
//
int exaSettingsInit(exaSettings *settings){
  exaMalloc(1,settings);
  exaArrayInit(&(*settings)->settings,struct exaSetting_private,10);

  (*settings)->info.objectType=exaSettingsObj;

  return 0;
}

int exaSettingsFree(exaSettings settings){
  exaArrayFree(settings->settings);
  exaFree(settings);
  return 0;
}

const char *exaGetSetting(const char *settingName,exaHandle h){
  return exaSettingsGetSetting(settingName,h->settings);
}
const char *exaSettingsGetSetting(const char *settingName,exaSettings s){
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++)
    if(strcmp(ptr[i].key,settingName)==0) break;

  if(i<size) return ptr[i].value;
  else return "\0";
}

int exaSetSetting(const char *settingName,const char *value,exaHandle h){
  return exaSettingsSetSetting(settingName,value,h->settings);
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
