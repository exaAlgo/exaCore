#ifndef _EXA_TYPES_H_
#define _EXA_TYPES_H_

#include <exa-gslib.h>
//
// exa types
//
typedef long long exaLong;
typedef unsigned long long exaULong;
#define exaLongFormat "%lld"
#define exaULongFormat "%llu"
#define exaLongMAX LLONG_MAX
#define exaLongMIN LLONG_MIN
#define exaULongMAX ULLONG_MAX
#define exaULongMIN 0

#define EXA_LONG MPI_LONG_LONG
#define EXA_UNSIGNED_LONG MPI_UNSIGNED_LONG_LONG

#define exa_gs_long gs_long_long

typedef int exaInt;
typedef unsigned int exaUInt;
#define exaIntFormat "%d"
#define exaUIntFormat "%u"
#define exaIntMAX INT_MAX
#define exaIntMIN INT_MIN
#define exaUIntMAX UINT_MAX
#define exaUIntMIN 0

#define EXA_INT MPI_INT
#define EXA_UNSIGNED_INT MPI_UNSIGNED_INT

#define exa_gs_int gs_int

typedef double exaScalar;
#define exa_gs_scalar gs_double
#define exaScalarFormat "%lf"
#define exaScalarMAX DBL_MAX
#define exaScalarMIN DBL_MIN

#define EXA_SCALAR MPI_DOUBLE

#endif
