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
  exaKernelCreate(p,"addVectors",&k);

  exaVector vecIn1,vecIn2,vecOut;
  exaVectorCreate(h,M,exaScalar_t,&vecIn1);
  exaVectorCreate(h,M,exaScalar_t,&vecIn2);
  exaVectorCreate(h,M,exaScalar_t,&vecOut);

  exaScalar *in1,*in2,*out;
  exaCalloc(M,&in1); exaCalloc(M,&in2);
  int i;
  for(i=0;i<M;i++){
    in1[i]=i+0.1;
    in2[i]=1.0-(i+0.1);
  }
  exaVectorWrite(vecIn1,in1);
  exaVectorWrite(vecIn2,in2);

  exaKernelRun(k,getExaInt(M),vecIn1,vecIn2,vecOut);

  exaVectorRead(vecOut,(void**)&out);
  for(i=0;i<M;i++){
    exaScalar ans=1.0;
    if(fabs(ans-out[i])>EXA_TOL)
      fprintf(stderr,"Error %lf != %lf\n",ans,out[i]);
  }

  exaFree(in1); exaFree(in2);

  exaDestroy(vecIn1); exaDestroy(vecIn2); exaDestroy(vecOut);
  exaDestroy(k);
  exaDestroy(p);
  exaDestroy(s);

  exaFinalize(h);

  MPI_Finalize();

  return 0;
}
