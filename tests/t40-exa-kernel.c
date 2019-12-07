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
  exaKernelCreate(p,"scaleVector",&k);

  exaVector vecIn,vecOut;
  exaVectorCreate(h,M,&vecIn);
  exaVectorCreate(h,M,&vecOut);

  exaScalar *in,*out;
  exaCalloc(M,&in); exaCalloc(M,&out);
  int i;
  for(i=0;i<M;i++)
    in[i]=i+0.1;
  exaVectorWrite(vecIn,in);

  exaKernelRun(k,getExaInt(M),getExaScalar(0.5),vecIn,vecOut);

  exaVectorRead(vecOut,out);
  for(i=0;i<M;i++){
    exaScalar ans=0.5*(i+0.1);
    if(fabs(ans-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",ans,out[i]);
  }

  exaFree(in); exaFree(out);
  exaDestroy(vecIn); exaDestroy(vecOut);
  exaDestroy(k);
  exaDestroy(p);
  exaDestroy(s);
  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
