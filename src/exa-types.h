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

#define EXA_LONG MPI_LONG_LONG
#define EXA_UNSIGNED_LONG MPI_UNSIGNED_LONG_LONG

#define exa_gs_long gs_long_long

typedef int exaInt;
typedef unsigned int exaUInt;
#define exaIntFormat "%d"
#define exaUIntFormat "%u"

#define EXA_INT MPI_INT
#define EXA_UNSIGNED_INT MPI_UNSIGNED_INT

#define exa_gs_int gs_int

typedef double exaScalar;
#define exa_gs_scalar gs_double
#define exaScalarFormat "%lf"

#define EXA_SCALAR MPI_DOUBLE

#endif
