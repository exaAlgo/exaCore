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
  exaKernelCreate(p,"setVector",&k);

  exaVector vec; exaVectorCreate(h,M,exaScalar_t,&vec);
  exaKernelRun(k,getExaInt(M),getExaScalar(0.5),vec);

  exaScalar *out; exaVectorRead(vec,(void**)&out);
  int i;
  for(i=0;i<M;i++){
    exaScalar ans=0.5;
    if(fabs(ans-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",ans,out[i]);
  }

  exaDestroy(vec);
  exaDestroy(k);
  exaDestroy(p);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
