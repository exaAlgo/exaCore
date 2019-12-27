#include "exa.h"
#include "exa-memory.h"

#include <math.h>

#define M 10
#define GET_OKL_NAME(sin,sout) do{\
  const char *slash=strrchr(sin,'-');\
  int length=strlen(sin)-strlen(slash);\
  strncpy(sout,sin,length);\
  sout[length]='\0';\
} while(0)

int main(int argc,char *argv[])
{
  MPI_Init(NULL,NULL);

  if(argc!=2){
    printf("Usage: %s <backend>\n",argv[0]);
    exit(0);
  }

  exaHandle h;
  exaInit(&h,MPI_COMM_WORLD,argv[1]);

  exaSettings s; exaSettingsCreate(h,NULL,&s);

  exaProgram p; char okl[BUFSIZ]; GET_OKL_NAME(argv[0],okl);
  exaProgramCreate(h,okl,s,&p);

  exaKernel k;
  exaKernelCreate(p,"addVectors",&k);

  exaVector vecIn1,vecIn2,vecOut;
  exaVectorCreate(h,M,exaScalar_t,&vecIn1);
  exaVectorCreate(h,M,exaScalar_t,&vecIn2);
  exaVectorCreate(h,M,exaScalar_t,&vecOut);

  exaScalar *in1,*in2,*out;
  exaCalloc(M,&in1); exaCalloc(M,&in2); exaCalloc(M,&out);
  int i;
  for(i=0;i<M;i++){
    in1[i]=i+0.1;
    in2[i]=1.0-(i+0.1);
  }
  exaVectorWrite(vecIn1,in1);
  exaVectorWrite(vecIn2,in2);

  exaKernelRun(k,getExaInt(M),vecIn1,vecIn2,vecOut);

  exaVectorRead(vecOut,out);
  for(i=0;i<M;i++){
    exaScalar ans=1.0;
    if(fabs(ans-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",ans,out[i]);
  }

  exaFree(in1); exaFree(in2); exaFree(out);

  exaDestroy(vecIn1); exaDestroy(vecIn2); exaDestroy(vecOut);
  exaDestroy(k);
  exaDestroy(p);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
