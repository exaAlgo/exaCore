#include <exa.h>
#include <mpi.h>

typedef struct{
  exaInt data;
  exaUInt proc;
} Data;

#define N 5

int main(int argc,char *argv[]){
  MPI_Init(&argc,&argv);

  exaSettings s; exaSettingsInit(&s);
  exaSettingsSetSetting("backend","/opencl/gpu",s);
  exaSettingsSetSetting("debug","on",s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);
  exaInt rank=exaRank(h);
  exaInt size=exaSize(h);

  exaArray arr;
  exaArrayInit(&arr,Data,1);

  exaInt i;
  Data d;
  for(int i=0;i<N;i++){
    d.data=(rank+1)%size;
    d.proc=(rank+1)%size; // send data to neighbor in a circle.
    exaArrayAppend(arr,&d);
  }

  exaArrayTransfer(arr,offsetof(Data,proc),1,exaGetComm(h));

  // check if rank i recieved the right data in right amount.
  assert(exaArrayGetSize(arr)==N);
  Data *ptr=exaArrayGetPointer(arr);
  for(int i=0;i<N;i++){
    assert(ptr[i].data==rank);
  }
  
  exaArrayTransfer(arr,offsetof(Data,proc),1,exaGetComm(h));

  assert(exaArrayGetSize(arr)==N);
  ptr=exaArrayGetPointer(arr);
  int orig=(rank+1)%size;
  for(int i=0;i<N;i++){
    assert(ptr[i].data==orig);
  }

  exaDestroy(s);
  exaDestroy(arr);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
