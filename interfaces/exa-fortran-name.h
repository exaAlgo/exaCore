#ifndef _EXA_FORTRAN_NAME_H_
#define _EXA_FORTRAN_NAME_H_

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

#endif
