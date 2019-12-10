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

int exaValueGetNative(void *p,exaValue val){
  //TODO: Check if this is redundant
  switch(val->type){
    case exaInt_t:
      *(exaInt*)p=val->value.i;
      break;
    case exaUInt_t:
      *(exaUInt*)p=val->value.ui;
      break;
    case exaLong_t:
      *(exaLong*)p=val->value.l;
      break;
    case exaULong_t:
      *(exaULong*)p=val->value.ul;
      break;
    case exaScalar_t:
      *(exaScalar*)p=val->value.s;
      break;
    case exaChar_t:
      *(exaChar*)p=val->value.b;
      break;
    case exaStr_t:
      *(void**)p=(void *)val->value.str;
      break;
    default:
      break;
  }

  return 0;
}

int exaValueEq(exaValue val1,exaValue val2)
{
  if(val1->type!=val2->type) return 0;

  switch(val1->type){
    case exaInt_t:
      return val1->value.i==val2->value.i;
    case exaUInt_t:
      return val1->value.ui==val2->value.ui;
    case exaLong_t:
      return val1->value.l==val2->value.l;
    case exaULong_t:
      return val1->value.ul==val2->value.ul;
    case exaScalar_t:
      return fabs(val1->value.s-val2->value.s)<EXA_TOL;
    case exaChar_t:
      return val1->value.b==val2->value.b;
    case exaStr_t:
      return strcmp(val1->value.str,val2->value.str)==0;
    default:
      break;
  }

  return 0;
}

exaValue getExaInt(const exaInt i){
  exaValue v;
  exaValueCreate(exaInt_t,(void*)&i,&v);
  return v;
}

exaValue getExaUInt(const exaUInt i){
  exaValue v;
  exaValueCreate(exaUInt_t,(void*)&i,&v);
  return v;
}

exaValue getExaLong(const exaLong i){
  exaValue v;
  exaValueCreate(exaLong_t,(void*)&i,&v);
  return v;
}

exaValue getExaULong(const exaULong i){
  exaValue v;
  exaValueCreate(exaULong_t,(void*)&i,&v);
  return v;
}

exaValue getExaScalar(const exaScalar i){
  exaValue v;
  exaValueCreate(exaScalar_t,(void*)&i,&v);
  return v;
}

exaValue getExaChar(const exaChar i){
  exaValue v;
  exaValueCreate(exaChar_t,(void*)&i,&v);
  return v;
}

exaValue getExaStr(const char *i){
  exaValue v;
  exaValueCreate(exaStr_t,(void*)i,&v);
  return v;
}

int exaValueFree(exaValue v){ exaFree(v); }
