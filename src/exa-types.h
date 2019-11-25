#ifndef _EXA_TYPES_H_
#define _EXA_TYPES_H_

#include <float.h>

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
#define exaLongMPIType  MPI_LONG_LONG
#define exaULongMPIType MPI_UNSIGNED_LONG_LONG
#define exaLongGSType gs_long
#define exaULongGSType gs_long
#define exaLongGSSortType 1
#define exaULongGSSortType 1

typedef int exaInt;
typedef unsigned int exaUInt;
#define exaIntFormat "%d"
#define exaUIntFormat "%u"
#define exaIntMAX INT_MAX
#define exaIntMIN INT_MIN
#define exaUIntMAX UINT_MAX
#define exaUIntMIN 0
#define exaIntMPIType  MPI_INT
#define exaUIntMPIType MPI_UNSIGNED
#define exaIntGSType gs_int
#define exaUIntGSType gs_int
#define exaIntGSSortType 0
#define exaUIntGSSortType 0

typedef double exaScalar;
#define exaScalarFormat "%lf"
#define exaScalarMAX DBL_MAX
#define exaScalarMIN DBL_MIN
#define exaScalarMPIType MPI_DOUBLE
#define exaScalarGSType gs_double
#define exaScalarGSSortType 3

typedef char exaByte;
#define exaByteMPIType MPI_BYTE

#define exaTypeGetMax(T) T##MAX
#define exaTypeGetMin(T) T##MIN
#define exaTypeGetMPIType(T) T##MPIType
#define exaTypeGetGSType(T) T##GSType
#define exaTypeGetGSSortType(T) T##GSSortType

#endif
