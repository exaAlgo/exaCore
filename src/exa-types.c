#include <exa-impl.h>
//
// exaDataType
//
size_t exaDataTypeGetSize(exaDataType t)
{
  size_t size;
  switch(t){
    case exaInt_t:
      size=sizeof(exaInt);
      break;
    case exaUInt_t:
      size=sizeof(exaUInt);
      break;
    case exaLong_t:
      size=sizeof(exaLong);
      break;
    case exaULong_t:
      size=sizeof(exaULong);
      break;
    case exaScalar_t:
      size=sizeof(exaScalar);
      break;
    case exaChar_t:
      size=sizeof(exaChar);
      break;
    default:
      break;
  }

  return size;
}

gs_dom exaDataTypeGetGSType(exaDataType t)
{
  gs_dom out;
  switch(t) {
    case exaInt_t:
      out=exaIntGSType;
      break;
    case exaUInt_t:
      out=exaUIntGSType;
      break;
    case exaLong_t:
      out=exaLongGSType;
      break;
    case exaULong_t:
      out=exaULongGSType;
      break;
    case exaScalar_t:
      out=exaScalarGSType;
      break;
    default:
      break;
  }
  return out;
}

MPI_Datatype exaDataTypeGetMPIType(exaDataType t)
{
  MPI_Datatype out;
  switch(t) {
    case exaInt_t:
      out=exaIntMPIType;
      break;
    case exaUInt_t:
      out=exaUIntMPIType;
      break;
    case exaLong_t:
      out=exaLongMPIType;
      break;
    case exaULong_t:
      out=exaULongMPIType;
      break;
    case exaScalar_t:
      out=exaScalarMPIType;
      break;
    case exaChar_t:
      out=exaCharMPIType;
      break;
    default:
      break;
  }
  return out;
}

void exaDataTypeGetMin(exaDataType t,void *out)
{
  switch(t) {
    case exaInt_t:
      *((exaInt *)out)=exaIntMIN;
      break;
    case exaUInt_t:
      *((exaUInt *)out)=exaUIntMIN;
      break;
    case exaLong_t:
      *((exaLong *)out)=exaLongMIN;
      break;
    case exaULong_t:
      *((exaULong *)out)=exaULongMIN;
      break;
    case exaScalar_t:
      *((exaScalar *)out)=exaScalarMIN;
      break;
    default:
      break;
  }
}

void exaDataTypeGetMax(exaDataType t,void *out)
{
  switch(t) {
    case exaInt_t:
      *((exaInt *)out)=exaIntMAX;
      break;
    case exaUInt_t:
      *((exaUInt *)out)=exaUIntMAX;
      break;
    case exaLong_t:
      *((exaLong *)out)=exaLongMAX;
      break;
    case exaULong_t:
      *((exaULong *)out)=exaULongMAX;
      break;
    case exaScalar_t:
      *((exaScalar *)out)=exaScalarMAX;
      break;
    default:
      break;
  }
}
