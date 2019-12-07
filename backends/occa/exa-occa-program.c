#include "exa-occa-impl.h"
#include "exa-memory.h"
//
// Create an Occa program
//
int exaOccaProgramCreate(exaProgram p,const char *fname,
  exaSettings kernelSettings)
{
  exaHandle h;
  exaOccaHandle oh;
  exaProgramGetHandle(p,&h);
  exaHandleGetData(h,(void**)&oh);

  exaOccaProgram op;
  exaCalloc(1,&op);

  op->props=occaCreateProperties();
  occaPropertiesSet(op->props,"defines/TILE_SIZE",occaInt(10));

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
  op=NULL;
  exaProgramSetData(p,(void**)&op);

  return 0;
}
