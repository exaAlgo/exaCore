#ifndef _EXA_FORTRAN_H_
#define _EXA_FORTRAN_H_

/* establishes some macros to establish
   * the FORTRAN naming convention
     default      gs_setup, etc.
     -DEXA_UPCASE     GS_SETUP, etc.
     -DEXA_UNDERSCORE gs_setup_, etc.
   * a prefix for all external (non-FORTRAN) function names
     for example, -DEXA_PREFIX=jl_   transforms fail -> jl_fail
   * a prefix for all external FORTRAN function names
     for example, -DEXA_FPREFIX=jlf_ transforms gs_setup_ -> jlf_gs_setup_
*/

/* the following macro functions like a##b,
   but will expand a and/or b if they are themselves macros */
#define EXA_TOKEN_PASTE_(a,b) a##b
#define EXA_TOKEN_PASTE(a,b) EXA_TOKEN_PASTE_(a,b)

#ifdef EXA_PREFIX
#  define EXA_PREFIXED_NAME(x) EXA_TOKEN_PASTE(PREFIX,x)
#else
#  define EXA_PREFIXED_NAME(x) x
#endif

#ifdef EXA_FPREFIX
#  define EXA_FPREFIXED_NAME(x) EXA_TOKEN_PASTE(FPREFIX,x)
#else
#  define EXA_FPREFIXED_NAME(x) x
#endif

#if defined(EXA_UPCASE)
#  define EXA_FORTRAN_NAME(low,up) EXA_FPREFIXED_NAME(up)
#  define EXA_FORTRAN_UNPREFIXED(low,up) up
#elif defined(EXA_UNDERSCORE)
#  define EXA_FORTRAN_NAME(low,up) EXA_FPREFIXED_NAME(TOKEN_PASTE(low,_))
#  define EXA_FORTRAN_UNPREFIXED(low,up) EXA_TOKEN_PASTE(low,_)
#else
#  define EXA_FORTRAN_NAME(low,up) EXA_FPREFIXED_NAME(low)
#  define EXA_FORTRAN_UNPREFIXED(low,up) low
#endif

typedef int exaFortranHandle;
typedef int exaFortranSettings;
typedef int exaFortranVector;
typedef int exaFortranProgram;
typedef int exaFortranKernel;

exaHandle exaHandleF2C(exaFortranHandle handle);
exaSettings exaSettingsF2C(exaFortranSettings settings);
exaVector exaVectorF2C(exaFortranVector vector);
exaProgram exaProgramF2C(exaFortranProgram program);
exaKernel exaKernelF2C(exaFortranKernel handle);

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
//
// Defines
//
#define EXA_NULL -1
#define EXA_MAX_STRLEN 2048

// Fortran strings are generally unterminated and the length is
// passed as an extra argument after all the normal arguments.
// Some compilers (I only know of Windows) place the length
// argument immediately after the string parameter (TODO). We can't
// just NULL-terminate the string in-place because that
// could overwrite other strings or attempt to write to read-only
// memory.  This macro allocates a string to hold the null-terminated
// version of the string that C expects.
#define EXA_FIX_STRING(stringname)\
  char EXA_TOKEN_PASTE(stringname,_c)[EXA_MAX_STRLEN];\
  if (EXA_TOKEN_PASTE(stringname,_len)>EXA_MAX_STRLEN-1)\
    fprintf(stderr,"Fortran string length too long %zd\n",\
      (size_t)EXA_TOKEN_PASTE(stringname,_len));\
  strncpy(EXA_TOKEN_PASTE(stringname,_c),stringname,\
    EXA_TOKEN_PASTE(stringname,_len));\
  EXA_TOKEN_PASTE(stringname,_c)[EXA_TOKEN_PASTE_(stringname,_len)]\
    =0;

#endif
