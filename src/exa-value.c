#include "exa-impl.h"
#include "exa-memory.h"
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
    case exaByte_t:
      val->value.b=*((exaByte *)ptr);
      break;
    default:
      break;
  }

  return 0;
}

exaValue getExaInt   (exaInt    i){exaValue v; exaValueCreate(exaInt_t   ,(void*)&i,&v); return v;}
exaValue getExaUInt  (exaUInt   i){exaValue v; exaValueCreate(exaUInt_t  ,(void*)&i,&v); return v;}
exaValue getExaLong  (exaLong   i){exaValue v; exaValueCreate(exaLong_t  ,(void*)&i,&v); return v;}
exaValue getExaULong (exaULong  i){exaValue v; exaValueCreate(exaULong_t ,(void*)&i,&v); return v;}
exaValue getExaScalar(exaScalar i){exaValue v; exaValueCreate(exaScalar_t,(void*)&i,&v); return v;}
exaValue getExaByte  (exaByte   i){exaValue v; exaValueCreate(exaByte_t  ,(void*)&i,&v); return v;}

int exaValueFree(exaValue v){ exaFree(v); }
