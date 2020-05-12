#include "exa-impl.h"
#include "exa-memory.h"

#include <assert.h>
//
// exaGS
//
int exaGSSetup(exaLong *ids,exaUInt n,exaComm c,int unique,
  int verbose,exaGS *t)
{
  exaMalloc(1,t);

  (*t)->indices=NULL;
  (*t)->uniqueIds=NULL;
  (*t)->offsets=NULL;

  (*t)->topology=gs_setup(ids,n,&c->gsComm,unique,gs_auto,verbose);
  (*t)->info.type=exaGSType;

  return 0;
}

typedef struct globalId_private{
  exaLong id;
  exaInt index;
  exaUInt offset;
} globalId;

int exaGSDeviceSetup(exaLong *ids,exaUInt n,exaComm c,int unique,
  int verbose,exaGS *t)
{
  exaGS s; exaGSSetup(ids,n,c,unique,verbose,&s);
  exaBuffer buf; exaBufferCreate(&buf,1024);

  exaInt *in; exaCalloc(n,&in);
  exaUInt i;
  for(i=0;i<n;i++) in[i]=1;

  exaGSOp(in,exaInt_t,exaAddOp,0,s,buf);

  exaArray globalIdsArray; exaArrayInit(&globalIdsArray,globalId,10);
  globalId gId;
  for(i=0;i<n;i++)
    if(in[i]>1){
      gId.id=ids[i],gId.index=i;
      exaArrayAppend(globalIdsArray,(void*)&gId);
    }

  exaFree(in);
  exaGSFree(s);

  // Allocate exaGS struct
  exaMalloc(1,t);

  //TODO: Use exaArraySort
  globalId *ptr=exaArrayGetPointer(globalIdsArray);
  exaUInt nGlobalIds=exaArrayGetSize(globalIdsArray);
  if(nGlobalIds==0){
    exaArrayFree(globalIdsArray);
    return 0;
  }

  sarray_sort_2(globalId,ptr,nGlobalIds,id,1,index,0,&buf->buf);

  exaUInt *activeIdxs; exaCalloc(nGlobalIds,&activeIdxs);
  for(i=0;i<nGlobalIds;i++) activeIdxs[i]=ptr[i].index;

  exaHandle h; exaCommGetHandle(c,&h);
  exaVectorCreate(h,nGlobalIds,exaUInt_t,&(*t)->indices);
  exaVectorWrite((*t)->indices,activeIdxs);
  exaFree(activeIdxs);

  exaArray uniqueIdsArray; exaArrayInit(&uniqueIdsArray,globalId,10);
  ptr[0].offset=0; exaArrayAppend(uniqueIdsArray,&ptr[0]);
  exaLong prev=ptr[0].id;
  for(i=1;i<nGlobalIds;i++)
    if(ptr[i].id!=prev){
      prev=ptr[i].id,ptr[i].offset=i;
      exaArrayAppend(uniqueIdsArray,&ptr[i]);
    }
   
  exaArrayFree(globalIdsArray);
  exaBufferFree(buf);

  exaUInt nUniqueIds=exaArrayGetSize(uniqueIdsArray);
  ptr=exaArrayGetPointer(uniqueIdsArray);

  exaVectorCreate(h,nUniqueIds  ,exaLong_t,&(*t)->uniqueIds);
  exaVectorCreate(h,nUniqueIds+1,exaUInt_t,&(*t)->offsets );

  exaLong *uniqueIds; exaCalloc(nUniqueIds  ,&uniqueIds);
  exaUInt *offsets  ; exaCalloc(nUniqueIds+1,&offsets  );
  for(i=0;i<nUniqueIds;i++)
    uniqueIds[i]=ptr[i].id,offsets[i]=ptr[i].offset;
  offsets[nUniqueIds]=nGlobalIds;

  exaArrayFree(uniqueIdsArray);

  exaVectorWrite((*t)->uniqueIds,uniqueIds);
  exaVectorWrite((*t)->offsets,offsets);

  (*t)->topology=gs_setup(uniqueIds,nUniqueIds,&c->gsComm,unique,
    gs_auto,verbose);
  (*t)->info.type=exaGSType;

  exaFree(uniqueIds);
  exaFree(offsets);

  return 0;
}

int exaGSGetIndices(exaGS gs,exaVector *indices){
  *indices=gs->indices;
  return 0;
}
int exaGSGetOffsets(exaGS gs,exaVector *offsets){
  *offsets=gs->offsets;
  return 0;
}

int exaGSOp(void *v,exaDataType T,exaOp op,unsigned transpose,
  exaGS t,exaBuffer buf)
{
  gs(v,exaDataTypeGetGSType(T),exaOpGetGSOp(op),transpose,
    t->topology,&buf->buf);
  return 0;
}

int exaGSFree(exaGS t){
  if(t==NULL) return 0;

  if(t->indices) exaVectorFree(t->indices);
  if(t->uniqueIds) exaVectorFree(t->uniqueIds);
  if(t->offsets) exaVectorFree(t->offsets);

  gs_free(t->topology);
  exaFree(t);

  return 0;
}
