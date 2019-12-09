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
  exaSettingsSet("backend::name",exaGetBackendName(h),s);

  exaSettingsSet("defines::exaLong",exaLongString,s);
  exaSettingsSet("defines::exaULong",exaULongString,s);
  exaSettingsSet("defines::exaInt",exaIntString,s);
  exaSettingsSet("defines::exaUInt",exaUIntString,s);
  exaSettingsSet("defines::exaScalar",exaScalarString,s);

  // TODO: read settings from file: fname

  s->info.type=exaSettingsType;

  return 0;
}

int exaSettingsFree(exaSettings settings){
  exaArrayFree(settings->settings);
  exaFree(settings);
  return 0;
}

const char *exaSettingsGet(const char *settingName,exaSettings s)
{
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting *ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++)
    if(strcmp(ptr[i].key,settingName)==0) break;

  if(i<size) return ptr[i].value;
  else return "\0";
}

int exaSettingsSet(const char *settingName,const char *value,
  exaSettings s)
{
  exaInt size=exaArrayGetSize(s->settings);
  exaSetting *ptr=exaArrayGetPointer(s->settings);

  exaInt i;
  for(i=0;i<size;i++) if(strcmp(ptr[i].key,settingName)==0) break;
  if(i<size) strcpy(ptr[i].value,value);
  else{
    exaSetting t;
    strcpy(t.key,settingName);
    strcpy(t.value,value);
    exaArrayAppend(s->settings,&t);
  }
}
