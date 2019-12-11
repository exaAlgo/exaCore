#include "exa-occa-impl.h"
#include "exa-memory.h"
//
// Create an Occa program
//
int exaOccaProgramCreate(exaProgram p,const char *fname,
  exaSettings s)
{
  exaHandle h;
  exaOccaHandle oh;
  exaProgramGetHandle(p,&h);
  exaHandleGetData(h,(void**)&oh);

  exaOccaProgram op;
  exaCalloc(1,&op);

  op->props=occaCreateProperties();

  exaValue val;
  const char *key;
  char str[BUFSIZ];

  const char *defines="defines::";
  key=exaSettingsIterateKeys(defines,s);
  while(key!=NULL){
    snprintf(str,BUFSIZ,"defines/%s",key+strlen(defines));
    exaDebug(h,"exa key: %s occa key: %s\n",key,str);

    val=exaSettingsGet(key,s);
    if(val!=NULL)
      occaPropertiesSet(op->props,str,exaValueToOccaType(val));

    key=exaSettingsIterateKeys(NULL,s);
  }

  const char *ext=exaOccaGetExt();
  exaMalloc(strlen(fname)+1+strlen(ext),&op->fileName);
  strcpy(op->fileName                  ,fname);
  strcpy(op->fileName+strlen(fname)    ,"."  );
  strcpy(op->fileName+strlen(fname)+1  ,ext  );
  exaDebug(h,"Source file name=%s\n",op->fileName);

  exaProgramSetData(p,(void**)&op);

  return 0;
}

int exaOccaProgramFree(exaProgram p){
  exaOccaProgram op;
  exaProgramGetData(p,(void**)&op);

  exaFree(op->fileName);
  occaFree(op->props);

  exaFree(op);

  return 0;
}
