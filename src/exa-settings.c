#include <exa-impl.h>
#include <exa-memory.h>
//
// exaSettings
//
int exaSettingsInit(exaHandle h,const char *fname,
  exaSettings *settings)
{
  exaMalloc(1,settings);
  exaSettings s=*settings;

  exaArrayInit(&s->settings,exaSetting,10);

  // set some defaults
  const char *name=exaGetBackendName(h);
  exaSettingsSet("backend::name",getExaStr(name),s);

  exaSettingsSet("defines::exaLong"  ,getExaStr(exaLongString  ),s);
  exaSettingsSet("defines::exaULong" ,getExaStr(exaULongString ),s);
  exaSettingsSet("defines::exaInt"   ,getExaStr(exaIntString   ),s);
  exaSettingsSet("defines::exaUInt"  ,getExaStr(exaUIntString  ),s);
  exaSettingsSet("defines::exaScalar",getExaStr(exaScalarString),s);

  // get backend specific settings
  h->updateSettings(s);

  // TODO: read settings from file: fname

  s->info.type=exaSettingsType;
  s->h=h;

  return 0;
}

int exaSettingsGetHandle(exaSettings s,exaHandle *h){
  *h=s->h;
  return 0;
}

int exaSettingsFree(exaSettings settings){
  exaArrayFree(settings->settings);
  exaFree(settings);
  return 0;
}

exaValue exaSettingsGet(const char *settingName,exaSettings s)
{
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting *ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++)
    if(strcmp(ptr[i].key,settingName)==0) break;

  if(i<size) return ptr[i].value;
  else return NULL;
}

int exaSettingsGetNative(void *out,const char *key,exaSettings s){
  exaValue v=exaSettingsGet(key,s);
  int err=exaValueGetNative(out,v);
  return err;
}

const char *exaSettingsIterateKeys(const char *startsWith,
  exaSettings s)
{
  static exaInt count=0;
  static int initialized=0;
  static const char *prefix;

  // initialize if startsWith not null
  if(startsWith!=NULL){
    initialized=1; count=0;
    prefix=startsWith;
  }

  exaInt size=exaArrayGetSize(s->settings);
  exaSetting *ptr=exaArrayGetPointer(s->settings);

  if(initialized)
    for(;count<size;count++)
      if(strncmp(ptr[count].key,prefix,strlen(prefix))==0) break;

  if(initialized && count<size)
    return ptr[count++].key;
  return  NULL;
}

int exaSettingsSet(const char *settingName,exaValue value,
  exaSettings s)
{
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting *ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++) if(strcmp(ptr[i].key,settingName)==0) break;
  if(i<size) ptr[i].value=value;
  else{
    exaSetting t;
    strcpy(t.key,settingName);
    t.value=value;
    exaArrayAppend(s->settings,&t);
  }
}
