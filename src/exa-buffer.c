#include "exa-impl.h"
#include "exa-memory.h"

int exaBufferCreate(exaBuffer *buf,size_t size){
  exaMalloc(1,buf);
  buffer_init(&(*buf)->buf,size);

  strcpy((*buf)->info.name,"exaBuffer");
}

int exaBufferFree(exaBuffer buf){
  buffer_free(&buf->buf);
  exaFree(buf);
}
