#include "exa-impl.h"
#include "exa-memory.h"

int exaBufferCreate(exaBuffer *buf,size_t size){
  exaMalloc(1,buf);
  if(size>0) buffer_init(&(*buf)->buf,size);
  else (*buf)->buf.ptr=0,(*buf)->buf.n=0,(*buf)->buf.max=0;

  (*buf)->info.objectType=exaBufferObj;
}

int exaBufferFree(exaBuffer buf){
  buffer_free(&buf->buf);
  exaFree(buf);
}
