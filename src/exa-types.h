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

#define exaLongCommType  MPI_LONG_LONG
#define exaULongCommType MPI_UNSIGNED_LONG_LONG
#define exaLongCommType  MPI_LONG_LONG
#define exaLongType 1

typedef int exaInt;
typedef unsigned int exaUInt;
#define exaIntFormat "%d"
#define exaUIntFormat "%u"
#define exaIntMAX INT_MAX
#define exaIntMIN INT_MIN
#define exaUIntMAX UINT_MAX
#define exaUIntMIN 0

#define exaIntCommType  MPI_INT
#define exaUIntCommType MPI_UNSIGNED_INT
#define exaIntGSType gs_int
#define exaIntType 0

typedef double exaScalar;
#define exaScalarFormat "%lf"
#define exaScalarMAX DBL_MAX
#define exaScalarMIN DBL_MIN

#define exaScalarCommType MPI_DOUBLE
#define exaScalarGSType gs_double
#define exaScalarType 3

#define exaTypeGetMax(T) T##MAX
#define exaTypeGetMin(T) T##MIN
#define exaTypeGetType(T) T##Type
#define exaTypeGetCommType(T) T##CommType
#define exaTypeGetGSType(T) T##GSType

#endif
