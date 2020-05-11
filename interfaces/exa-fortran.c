#include <exa-impl.h>
#include <exa-memory.h>
#include <exa-fortran.h>

#include <stdlib.h>
#include <string.h>

static exaHandle *handleDict=NULL;
static int handleCurrent=0;
static int handleActive=0;
static int handleMax=0;

#define fExaInit EXA_FORTRAN_NAME(exainit,EXAINIT)
void fExaInit(const char *backend,MPI_Fint *fcomm,
  exaFortranHandle *exa,int *err,fortran_charlen_t backend_len)
{
  EXA_FIX_STRING(backend);
  if(handleCurrent==handleMax)
    handleMax+=handleMax/2+1,exaRealloc(handleMax,&handleDict);

  exaExternalComm comm;
#if defined(EXA_MPI)
  comm=MPI_Comm_f2c(*fcomm);
#else
  comm=1;
#endif

  *err=exaInit(&handleDict[handleCurrent],comm,backend_c);

  if(*err==0)
    *exa=handleCurrent++,handleActive++;
}

#define fExaFinalize EXA_FORTRAN_NAME(exafinalize,EXAFINALIZE)
void fExaFinalize(exaFortranHandle *exa,int *err){
  *err=exaFinalize(exaHandleF2C(*exa));

  if(*err==0){
    handleActive--;
    if(handleActive==0)
      exaFree(handleDict),handleCurrent=0,handleMax=0;
  }
}

static exaSettings *settingsDict=NULL;
static int settingsCurrent=0;
static int settingsActive=0;
static int settingsMax=0;

#define fSettingsCreate\
  EXA_FORTRAN_NAME(exasettingscreate,EXASETTINGSCREATE)
void fSettingsCreate(exaFortranHandle *exa,const char *fname,
  exaFortranSettings *s,int *err,fortran_charlen_t fname_len)
{
  EXA_FIX_STRING(fname);

  if(settingsCurrent==settingsMax){
    settingsMax+=settingsMax/2+1;
    exaRealloc(settingsMax,&settingsDict);
  }

  *err=exaSettingsCreate(exaHandleF2C(*exa),
    strlen(fname_c)>0?fname_c:NULL,&settingsDict[settingsCurrent]);

  if(*err==0)
    *s=settingsCurrent++,settingsActive++;
}

#define fSettingsSetInt\
  EXA_FORTRAN_NAME(exasettingssetint,EXASETTINGSSETINT)
void fSettingsSetInt(const char *sname,int *ival,
  exaFortranSettings *s,int *err,fortran_charlen_t sname_len)
{
  EXA_FIX_STRING(sname);
  *err=exaSettingsSet(sname_c,getExaInt(*ival),exaSettingsF2C(*s));
}

#define fSettingsSetStr\
  EXA_FORTRAN_NAME(exasettingssetstr,EXASETTINGSSETSTR)
void fSettingsSetStr(const char *sname,const char *sval,
  exaFortranSettings *s,int *err,fortran_charlen_t sname_len,
  fortran_charlen_t sval_len)
{
  EXA_FIX_STRING(sname);
  EXA_FIX_STRING(sval);

  *err=exaSettingsSet(sname_c,getExaStr(sval_c),exaSettingsF2C(*s));
}

#define fSettingsGetStr\
  EXA_FORTRAN_NAME(exasettingsgetstr,EXASETTINGSGETSTR)
void fSettingsGetStr(char *sval,const char *sname,
  exaFortranSettings *s,int *err,fortran_charlen_t sval_len,
  fortran_charlen_t sname_len)
{
  EXA_FIX_STRING(sname);

  char *val;
  *err=exaSettingsGet(&val,sname_c,exaSettingsF2C(*s));
  memset(sval,' ',sval_len);
  strcpy(sval,val),sval[strlen(sval)]=' ';
}

#define fSettingsGetInt\
  EXA_FORTRAN_NAME(exasettingsgetint,EXASETTINGSGETint)
void fSettingsGetInt(int *ival,const char *sname,
  exaFortranSettings *s,int *err,fortran_charlen_t sname_len)
{
  EXA_FIX_STRING(sname);
  *err=exaSettingsGet(ival,sname_c,exaSettingsF2C(*s));
}

#define fSettingsFree EXA_FORTRAN_NAME(exasettingsfree,\
  EXASETTINGSFREE)
void fSettingsFree(exaFortranSettings *s,int *err){
  *err=exaSettingsFree(exaSettingsF2C(*s));

  if(*err==0){
    settingsActive--;
    if(settingsActive==0)
      exaFree(settingsDict),settingsCurrent=0,settingsMax=0;
  }
}

static exaVector *vectorDict=NULL;
static int vectorCurrent=0;
static int vectorActive=0;
static int vectorMax=0;

#define fExaVectorCreate\
  EXA_FORTRAN_NAME(exavectorcreate,EXAVECTORCREATE)
void fExaVectorCreate(exaFortranHandle *exa,int *length,
  int *type,exaFortranVector *vec,int *err)
{
  if(vectorCurrent==vectorMax){
    vectorMax+=vectorMax/2+1;
    exaRealloc(vectorMax,&vectorDict);
  }

  *err=exaVectorCreate(exaHandleF2C(*exa),*length,*type,
    &vectorDict[vectorCurrent]);

  if(*err==0)
    *vec=vectorCurrent++,vectorActive++;
}

#define fExaVectorGetSize\
  EXA_FORTRAN_NAME(exavectorgetsize,EXAVECTORGETSIZE)
void fExaVectorGetSize(int *size,exaFortranVector *vec,int *err)
{
  *size=exaVectorGetSize(exaVectorF2C(*vec));
  *err=0;
}

#define fExaVectorRead EXA_FORTRAN_NAME(exavectorread,EXAVECTORREAD)
void fExaVectorRead(exaFortranVector *vec,exaScalar *array,
  ptrdiff_t *offset,int *err)
{
  exaVector v=exaVectorF2C(*vec);
  exaScalar *b;
  *err=exaVectorRead(v,(void**)&b);
  *offset=(ptrdiff_t)(b-array);
}

#define fExaVectorWrite\
  EXA_FORTRAN_NAME(exavectorwrite,EXAVECTORWRITE)
void fExaVectorWrite(exaFortranVector *vec,exaScalar *array,
  int64_t *offset,int *err)
{
  *err=exaVectorWrite(exaVectorF2C(*vec),(array+*offset));
}

#define fExaVectorFree EXA_FORTRAN_NAME(exavectorfree,EXAVECTORFREE)
void fExaVectorFree(exaFortranVector *vec,int *err){
  *err=exaVectorFree(exaVectorF2C(*vec));

  if(*err==0){
    vectorActive--;
    if(vectorActive==0)
      exaFree(vectorDict),vectorCurrent=0,vectorMax=0;
  }
}

static exaProgram *programDict=NULL;
static int programCurrent=0;
static int programActive=0;
static int programMax=0;

#define fExaProgramCreate\
  EXA_FORTRAN_NAME(exaprogramcreate,EXAPROGRAMCREATE)
void fExaProgramCreate(exaFortranHandle *exa,const char *fname,
  exaFortranSettings *s,exaFortranProgram *prog,int *err,
  fortran_charlen_t fname_len)
{
  EXA_FIX_STRING(fname);

  if(programCurrent==programMax){
    programMax+=programMax/2+1;
    exaRealloc(programMax,&programDict);
  }

  *err=exaProgramCreate(exaHandleF2C(*exa),fname_c,
    exaSettingsF2C(*s),&programDict[programCurrent]);

  if(*err==0)
    *prog=programCurrent++,programActive++;
}

#define fExaProgramFree\
  EXA_FORTRAN_NAME(exaprogramfree,EXAPROGRAMFREE)
void fExaProgramFree(exaFortranProgram *prog,int *err){
  *err=exaProgramFree(exaProgramF2C(*prog));

  if(*err==0){
    programActive--;
    if(programActive==0)
      exaFree(programDict),programCurrent=0,programMax=0;
  }
}

static exaKernel *kernelDict=NULL;
static int kernelCurrent=0;
static int kernelActive=0;
static int kernelMax=0;

#define fExaKernelCreate\
  EXA_FORTRAN_NAME(exakernelcreate,EXAKERNELCREATE)
void fExaKernelCreate(exaFortranProgram *prog,const char *knlName,
  exaFortranKernel *knl,int *err,fortran_charlen_t knlName_len)
{
  EXA_FIX_STRING(knlName);

  if(kernelCurrent==kernelMax){
    kernelMax+=kernelMax/2+1;
    exaRealloc(kernelMax,&kernelDict);
  }

  *err=exaKernelCreate(exaProgramF2C(*prog),knlName_c,
    &kernelDict[kernelCurrent]);

  if(*err==0)
    *knl=kernelCurrent++,kernelActive++;
}

//TODO: exaKernelRun

#define fExaKernelFree\
  EXA_FORTRAN_NAME(exakernelfree,EXAKERNELFREE)
void fExaKernelFree(exaFortranKernel *knl,int *err){
  *err=exaKernelFree(exaKernelF2C(*knl));

  if(*err==0){
    kernelActive--;
    if(kernelActive==0)
      exaFree(kernelDict),kernelCurrent=0,kernelMax=0;
  }
}

exaHandle exaHandleF2C(exaFortranHandle handle){
  if(handle<0||handle>=handleCurrent||handleActive<=0){
    fprintf(stderr,"Invalid exaFortranHandle.");
    return NULL;
  }
  return handleDict[handle];
}

exaSettings exaSettingsF2C(exaFortranSettings settings){
  if(settings<0||settings>=settingsCurrent||settingsActive<=0){
    fprintf(stderr,"Invalid exaFortranSettings.");
    return NULL;
  }
  return settingsDict[settings];
}

exaVector exaVectorF2C(exaFortranVector vector){
  if(vector<0||vector>=vectorCurrent||vectorActive<=0){
    fprintf(stderr,"Invalid exaFortranVector.");
    return NULL;
  }
  return vectorDict[vector];
}

exaProgram exaProgramF2C(exaFortranProgram program){
  if(program<0||program>=programCurrent||programActive<=0){
    fprintf(stderr,"Invalid exaFortranProgram.");
    return NULL;
  }
  return programDict[program];
}

exaKernel exaKernelF2C(exaFortranKernel kernel){
  if(kernel<0||kernel>=kernelCurrent||kernelActive<=0){
    fprintf(stderr,"Invalid exaFortranKernel.");
    return NULL;
  }
  return kernelDict[kernel];
}
