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

#define Splice(a, b) a ## b

// Fortran strings are generally unterminated and the length is
// passed as an extra argument after all the normal arguments.
// Some compilers (I only know of Windows) place the length
// argument immediately after the string parameter (TODO). We can't
// just NULL-terminate the string in-place because that
// could overwrite other strings or attempt to write to read-only
// memory.  This macro allocates a string to hold the null-terminated
// version of the string that C expects.
#define FIX_STRING(stringname)\
  char Splice(stringname, _c)[1024];\
  if (Splice(stringname, _len) > 1023)\
    fprintf(stderr,"Fortran string length too long %zd",\
      (size_t)Splice(stringname, _len));\
  strncpy(Splice(stringname,_c),stringname,Splice(stringname,_len));\
  Splice(stringname,_c)[Splice(stringname,_len)]=0;\

static exaHandle *handleDict=NULL;
static int handleCurrent=0;
static int handleActive=0;
static int handleMax=0;

#define fExaInit FORTRAN_NAME(exainit,EXAINIT)
void fExaInit(const char *backend,MPI_Fint *fcomm,int *exa,int *err,
  fortran_charlen_t backend_len)
{
  FIX_STRING(backend);
  if(handleCurrent==handleMax)
    handleMax+=handleMax/2+1,exaRealloc(handleMax,&handleDict);

  MPI_Comm comm=MPI_Comm_f2c(*fcomm);
  *err=exaInit(&handleDict[handleCurrent],comm,backend);

  if(*err==0)
    *exa=handleCurrent++,handleActive++;
}

#define fExaFinalize FORTRAN_NAME(exafinalize,EXAFINALIZE)
void fExaFinalize(int *exa, int *err){
  *err=exaFinalize(handleDict[*exa]);

  if(*err==0){
    handleActive--;
    if(handleActive==0)
      exaFree(handleDict),handleCurrent=0,handleMax=0;
  }
}

static exaVector *vectorDict=NULL;
static int vectorCurrent=0;
static int vectorActive=0;
static int vectorMax=0;

#define fexaVectorCreate FORTRAN_NAME(exavectorcreate,EXAVECTORCREATE)
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

#define fexaVectorRead FORTRAN_NAME(exavectorread,EXAVECTORREAD)
void fExaVectorRead(int *vec,exaScalar *array,int64_t *offset,
  int *err)
{
  const exaScalar *b;
  *err=exaVectorRead(vectorDict[*vec],&b);
  *offset=b-array;
}

#define fexaVectorWrite FORTRAN_NAME(exavectorread,EXAVECTORREAD)
void fExaVectorWrite(int *vec,exaScalar *array,int64_t *offset,
  int *err)
{
  *err=exaVectorWrite(vectorDict[*vec],(array+*offset));
}

#define fExaVectorFree FORTRAN_NAME(exavectorfree,EXAVECTORFREE)
void fExaVectorFree(int *vec,int *err){
  *err=exaVectorFree(vectorDict[*vec]);

  if(*err==0){
    vectorActive--;
    if(vectorActive==0)
      exaFree(&vectorDict),vectorCurrent=0,vectorMax= 0;
  }
}

