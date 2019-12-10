#include "exa-impl.h"
#include "exa-memory.h"

#include <string.h>

//
// exaValue
//
int exaValueCreate(exaDataType t,void *ptr,exaValue *val_)
{
  exaMalloc(1,val_);
  exaValue val=*val_;

  val->info.type=exaValueType;
  val->type=t;
  switch(t){
    case exaInt_t:
      val->value.i=*((exaInt *)ptr);
      break;
    case exaUInt_t:
      val->value.ui=*((exaUInt *)ptr);
      break;
    case exaLong_t:
      val->value.l=*((exaLong *)ptr);
      break;
    case exaULong_t:
      val->value.ul=*((exaULong *)ptr);
      break;
    case exaScalar_t:
      val->value.s=*((exaScalar *)ptr);
      break;
    case exaChar_t:
      val->value.b=*((exaChar *)ptr);
      break;
    case exaStr_t:
      exaCalloc(strlen((exaStr)ptr)+1,&val->value.str);
      strcpy(val->value.str,(exaStr)ptr);
      break;
    default:
      break;
  }

  return 0;
}

exaValue getExaInt(exaInt i){
  exaValue v;
  exaValueCreate(exaInt_t,(void*)&i,&v);
  return v;
}

exaValue getExaUInt(exaUInt i){
  exaValue v;
  exaValueCreate(exaUInt_t,(void*)&i,&v);
  return v;
}

exaValue getExaLong(exaLong i){
  exaValue v;
  exaValueCreate(exaLong_t,(void*)&i,&v);
  return v;
}

exaValue getExaULong(exaULong i){
  exaValue v;
  exaValueCreate(exaULong_t,(void*)&i,&v);
  return v;
}

exaValue getExaScalar(exaScalar i){
  exaValue v;
  exaValueCreate(exaScalar_t,(void*)&i,&v);
  return v;
}

exaValue getExaChar(exaChar i){
  exaValue v;
  exaValueCreate(exaChar_t,(void*)&i,&v);
  return v;
}

exaValue getExaStr(exaStr i){
  exaValue v;
  exaValueCreate(exaStr_t,(void*)i,&v);
  return v;
}

int exaValueFree(exaValue v){ exaFree(v); }
