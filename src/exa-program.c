#include "exa-impl.h"
#include "exa-memory.h"
#include <stdio.h>

int exaProgramCreate(exaHandle h,const char *fname,exaProgram *p_){
  exaMalloc(1,p_);
  exaProgram p=*p_;

  p->handle=h;

  int length=strlen(fname);
  exaMalloc(length+1,&p->fname);
  strcpy(p->fname,fname);

  h->programCreate(p,fname);

  strcpy(p->info.name,"exaProgram");

  return 0;
}

int exaProgramGetHandle(exaProgram p,exaHandle *h){
  *h=p->handle;
}

int exaProgramGetData(exaProgram p,void **data){
  *data=p->data;
}

int exaProgramSetData(exaProgram p,void **data){
  p->data=*data;
}

int exaProgramFree(exaProgram p){
  exaHandle h;
  exaProgramGetHandle(p,&h);

  h->programFree(p);

  exaFree(p->fname);
  exaFree(p);
}
