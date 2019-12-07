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

  exaVector vec;
  exaVectorCreate(h,M,&vec);

  if(M!=exaVectorGetSize(vec))
    fprintf(stderr,"exaVectorGetSize failed: %d != %d\n",M,exaVectorGetSize(vec));

  exaScalar *in,*out;
  exaCalloc(M,&in); exaCalloc(M,&out);

  int i;
  for(i=0;i<M;i++)
    in[i]=i+0.1;
  exaVectorWrite(vec,in);

  exaVectorRead(vec,out);
  for(i=0;i<M;i++)
    if(fabs(i+0.1-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",i+0.1,out[i]);

  exaFree(in);
  exaFree(out);
  exaDestroy(vec);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
