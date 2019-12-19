#include "exa-impl.h"
#include "exa-memory.h"
#include <stdio.h>

int exaProgramCreate(exaHandle h,const char *fname,exaSettings settings,exaProgram *p_){
  exaMalloc(1,p_);
  exaProgram p=*p_;

  p->handle=h;

  int length=strlen(fname);
  exaMalloc(length+1,&p->fname);
  strcpy(p->fname,fname);

  h->programCreate(p,fname,settings);
  p->info.type=exaProgramType;

  return 0;
}

int exaProgramGetHandle(exaProgram p,exaHandle *h){
  *h=p->handle;
  return 0;
}

int exaProgramGetData(exaProgram p,void **data){
  *data=p->data;
  return 0;
}

int exaProgramSetData(exaProgram p,void **data){
  p->data=*data;
  return 0;
}

int exaProgramFree(exaProgram p){
  if(p==NULL) return 0;

  exaHandle h;
  exaProgramGetHandle(p,&h);
  h->programFree(p);
  exaFree(p->fname);
  exaFree(p);

  return 0;
}
