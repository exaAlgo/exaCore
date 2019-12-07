#include "exa-impl.h"
#include "exa-memory.h"

int exaBufferCreate(exaBuffer *buf_,size_t size){
  exaMalloc(1,buf_);
  exaBuffer buf=*buf_;

  if(size>0) buffer_init(&buf->buf,size);
  else buf->buf.ptr=0,buf->buf.n=0,buf->buf.max=0;

  buf->info.type=exaBufferType;
}

int exaBufferFree(exaBuffer buf){
  buffer_free(&buf->buf);
  exaFree(buf);
}
