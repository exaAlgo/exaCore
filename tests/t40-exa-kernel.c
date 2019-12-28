#include "exa.h"
#include "exa-memory.h"

#include <math.h>

#define M 10
#define KERNEL_FILENAME(sin,sout) do{\
  int length=strlen(sin);\
  strncpy(sout,sin,length-2);\
  sout[length-2]='\0';\
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

  exaProgram p; char knlFname[BUFSIZ];
  KERNEL_FILENAME(argv[0],knlFname);
  exaProgramCreate(h,knlFname,s,&p);

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
