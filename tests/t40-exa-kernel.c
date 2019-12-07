#include "exa.h"
#include "exa-memory.h"

#include <math.h>

#define M 10

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaSettings s;
  exaSettingsInit(&s);
  exaSettingsSetSetting("backend",argv[1],s);

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,s);

  exaProgram p;
  exaProgramCreate(h,argv[0],s,&p);

  exaKernel k;
  exaKernelCreate(p,"setVector",&k);

  exaVector vec; exaVectorCreate(h,M,&vec);
  exaKernelRun(k,getExaInt(M),getExaScalar(0.5),vec);

  exaScalar *out; exaCalloc(M,&out);
  exaVectorRead(vec,out);
  int i;
  for(i=0;i<M;i++){
    exaScalar ans=0.5;
    if(fabs(ans-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",ans,out[i]);
  }

  exaFree(out);

  exaDestroy(vec);
  exaDestroy(k);
  exaDestroy(p);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}