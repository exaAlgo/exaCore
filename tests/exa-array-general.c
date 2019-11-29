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
  assert(exaArrayGetSize(arr)==0);

  exaInt i;
  Data d;
  for(int i=0;i<N;i++){
    d.data=(rank+1)%size;
    d.proc=(rank+1)%size;
    exaArrayAppend(arr,&d);
  }

  assert(exaArrayGetSize(arr)==N);
  assert(exaArrayGetMaxSize(arr)>=N);
  assert(exaArrayGetUnitSize(arr)==sizeof(Data));

  exaArrayResize(arr,4*N);
  exaInt max=exaArrayGetMaxSize(arr);

  for(int i=0;i<3*N-1;i++){
    d.data=(rank+1)%size;
    d.proc=(rank+1)%size;
    exaArrayAppend(arr,&d);
  }

  assert(exaArrayGetSize(arr)==4*N-1);
  assert(exaArrayGetMaxSize(arr)==max);

  exaDestroy(s);
  exaDestroy(arr);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
