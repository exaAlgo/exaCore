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

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaVector vec;
  exaVectorCreate(h,M,exaScalar_t,&vec);

  if(M!=exaVectorGetSize(vec))
    fprintf(stderr,"exaVectorGetSize failed: %d != %d\n",M,
      exaVectorGetSize(vec));

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

  exaVector iVec;
  exaVectorCreate(h,M,exaInt_t,&iVec);

  if(M!=exaVectorGetSize(iVec))
    fprintf(stderr,"exaVectorGetSize failed: %d != %d\n",M,
      exaVectorGetSize(iVec));

  exaInt *iIn,*iOut;
  exaCalloc(M,&iIn); exaCalloc(M,&iOut);

  for(i=0;i<M;i++)
    iIn[i]=2*i+1;
  exaVectorWrite(iVec,iIn);

  exaVectorRead(iVec,iOut);
  for(i=0;i<M;i++)
    if(iOut[i]!=2*i+1)
      fprintf(stderr,"Error %d != %d\n",2*i+1,iOut[i]);

  exaFree(iIn);
  exaFree(iOut);
  exaDestroy(iVec);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
