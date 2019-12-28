// Fortran interface
#include <exa.h>
#include <exa-impl.h>
#include <exa-memory.h>
#include <exa-fortran-name.h>
#include <stdlib.h>
#include <string.h>

// This test should actually be for the gfortran version,
// but we don't currently have a configure system to determine
// that (TODO).  At present, this will use the smaller integer
// when run with clang+gfortran=8, for example.  (That is
// sketchy, but will likely work for users that don't have huge
// character strings.)
#if __GNUC__ >= 8
typedef size_t fortran_charlen_t;
#else
typedef int fortran_charlen_t;
#endif

// Fortran strings are generally unterminated and the length is
// passed as an extra argument after all the normal arguments.
// Some compilers (I only know of Windows) place the length
// argument immediately after the string parameter (TODO). We can't
// just NULL-terminate the string in-place because that
// could overwrite other strings or attempt to write to read-only
// memory.  This macro allocates a string to hold the null-terminated
// version of the string that C expects.
#define MAX_LEN 2048
#define FIX_STRING(stringname)\
  char EXA_TOKEN_PASTE(stringname,_c)[MAX_LEN];\
  if (EXA_TOKEN_PASTE(stringname,_len)>MAX_LEN-1)\
    fprintf(stderr,"Fortran string length too long %zd\n",\
      (size_t)EXA_TOKEN_PASTE(stringname,_len));\
  strncpy(EXA_TOKEN_PASTE(stringname,_c),stringname,\
    EXA_TOKEN_PASTE(stringname,_len));\
  EXA_TOKEN_PASTE(stringname,_c)[EXA_TOKEN_PASTE_(stringname,_len)]\
    =0;
//
// Defines
//
#define EXA_NULL -1

static exaHandle *handleDict=NULL;
static int handleCurrent=0;
static int handleActive=0;
static int handleMax=0;

#define fExaInit EXA_FORTRAN_NAME(exainit,EXAINIT)
void fExaInit(const char *backend,MPI_Fint *fcomm,int *exa,int *err,
  fortran_charlen_t backend_len)
{
  FIX_STRING(backend);
  if(handleCurrent==handleMax)
    handleMax+=handleMax/2+1,exaRealloc(handleMax,&handleDict);

  MPI_Comm comm=MPI_Comm_f2c(*fcomm);
  *err=exaInit(&handleDict[handleCurrent],comm,backend_c);

  if(*err==0)
    *exa=handleCurrent++,handleActive++;
}

#define fExaFinalize EXA_FORTRAN_NAME(exafinalize,EXAFINALIZE)
void fExaFinalize(int *exa, int *err){
  // TODO: Validate handle
  *err=exaFinalize(handleDict[*exa]);

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
void fSettingsCreate(int *exa,const char *fname,int *s,int *err,
  fortran_charlen_t fname_len)
{
  FIX_STRING(fname);

  if(settingsCurrent==settingsMax){
    settingsMax+=settingsMax/2+1;
    exaRealloc(settingsMax,&settingsDict);
  }

  // TODO: Validate handle
  *err=exaSettingsCreate(handleDict[*exa],
    strlen(fname_c)>0?fname_c:NULL,&settingsDict[settingsCurrent]);

  if(*err==0)
    *s=settingsCurrent++,settingsActive++;
}

#define fSettingsSetInt\
  EXA_FORTRAN_NAME(exasettingssetint,EXASETTINGSSETINT)
void fSettingsSetInt(const char *sname,int *ival,int *s,int *err,
  fortran_charlen_t sname_len)
{
  FIX_STRING(sname);

  // TODO: Validate Settings Handle
  *err=exaSettingsSet(sname_c,getExaInt(*ival),settingsDict[*s]);
}

#define fSettingsSetStr\
  EXA_FORTRAN_NAME(exasettingssetstr,EXASETTINGSSETSTR)
void fSettingsSetStr(const char *sname,const char *sval,int *s,
  int *err,fortran_charlen_t sname_len,fortran_charlen_t sval_len)
{
  FIX_STRING(sname);
  FIX_STRING(sval);

  // TODO: Validate Settings Handle
  *err=exaSettingsSet(sname_c,getExaStr(sval_c),settingsDict[*s]);
}

#define fSettingsGetStr\
  EXA_FORTRAN_NAME(exasettingsgetstr,EXASETTINGSGETSTR)
void fSettingsGetStr(char *sval,const char *sname,int *s,
  int *err,fortran_charlen_t sval_len,fortran_charlen_t sname_len)
{
  FIX_STRING(sname);

  // TODO: Validate Settings Handle
  char *val;
  *err=exaSettingsGet(&val,sname_c,settingsDict[*s]);
  memset(sval,' ',sval_len);
  strcpy(sval,val),sval[strlen(sval)]=' ';
}

#define fSettingsGetInt\
  EXA_FORTRAN_NAME(exasettingsgetint,EXASETTINGSGETint)
void fSettingsGetInt(int *ival,const char *sname,int *s,
  int *err,fortran_charlen_t sname_len)
{
  FIX_STRING(sname);

  // TODO: Validate Settings Handle
  *err=exaSettingsGet(ival,sname_c,settingsDict[*s]);
}

#define fSettingsFree EXA_FORTRAN_NAME(exasettingsfree,\
  EXASETTINGSFREE)
void fSettingsFree(int *s,int *err){
  *err=exaSettingsFree(settingsDict[*s]);

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
void fExaVectorCreate(int *exa,int *length,int *type,int *vec,
  int *err)
{
  if(vectorCurrent==vectorMax){
    vectorMax+=vectorMax/2+1;
    exaRealloc(vectorMax,&vectorDict);
  }

  *err=exaVectorCreate(handleDict[*exa],*length,*type,
    &vectorDict[vectorCurrent]);

  if(*err==0)
    *vec=vectorCurrent++,vectorActive++;
}

#define fExaVectorGetSize\
  EXA_FORTRAN_NAME(exavectorgetsize,EXAVECTORGETSIZE)
void fExaVectorGetSize(int *size,int *vec,int *err)
{
  *size=exaVectorGetSize(vectorDict[*vec]);
  *err=0;
}

#define fExaVectorRead EXA_FORTRAN_NAME(exavectorread,EXAVECTORREAD)
void fExaVectorRead(int *vec,exaScalar *array,int64_t *offset,
  int *err)
{
  exaScalar *b;
  *err=exaVectorRead(vectorDict[*vec],(void**)&b);
  *offset=b-array;
}

#define fExaVectorWrite\
  EXA_FORTRAN_NAME(exavectorwrite,EXAVECTORWRITE)
void fExaVectorWrite(int *vec,exaScalar *array,int64_t *offset,
  int *err)
{
  *err=exaVectorWrite(vectorDict[*vec],(array+*offset));
}

#define fExaVectorFree EXA_FORTRAN_NAME(exavectorfree,EXAVECTORFREE)
void fExaVectorFree(int *vec,int *err){
  *err=exaVectorFree(vectorDict[*vec]);

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
void fExaProgramCreate(int *exa,const char *fname,int *s,int *prog,
  int *err,fortran_charlen_t fname_len)
{
  FIX_STRING(fname);

  if(programCurrent==programMax){
    programMax+=programMax/2+1;
    exaRealloc(programMax,&programDict);
  }

  //TODO: Validate handles
  *err=exaProgramCreate(handleDict[*exa],fname_c,settingsDict[*s],
    &programDict[programCurrent]);

  if(*err==0)
    *prog=programCurrent++,programActive++;
}

#define fExaProgramFree\
  EXA_FORTRAN_NAME(exaprogramfree,EXAPROGRAMFREE)
void fExaProgramFree(int *prog,int *err){
  *err=exaProgramFree(programDict[*prog]);

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
void fExaKernelCreate(int *prog,const char *knlName,int *knl,
  int *err,fortran_charlen_t knlName_len)
{
  FIX_STRING(knlName);

  if(kernelCurrent==kernelMax){
    kernelMax+=kernelMax/2+1;
    exaRealloc(kernelMax,&kernelDict);
  }

  //TODO: Validate handles
  *err=exaKernelCreate(programDict[*prog],knlName_c,
    &kernelDict[kernelCurrent]);

  if(*err==0)
    *knl=kernelCurrent++,kernelActive++;
}

//TODO: exaKernelRun

#define fExaKernelFree\
  EXA_FORTRAN_NAME(exakernelfree,EXAKERNELFREE)
void fExaKernelFree(int *knl,int *err){
  *err=exaKernelFree(kernelDict[*knl]);

  if(*err==0){
    kernelActive--;
    if(kernelActive==0)
      exaFree(kernelDict),kernelCurrent=0,kernelMax=0;
  }
}
