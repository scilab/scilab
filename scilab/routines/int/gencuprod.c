#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define CUPROD(Type) {\
Type *W;\
    W=(Type *)w;\
    t = 1; kk = 0;\
    for (k = 0; k < *n; ++k) {\
        t *= (integer)W[kk];\
	W[kk] =(Type)t;kk += (*iw);\
    }\
}


int C2F(gencuprod)(typ,n, w, iw)
integer *typ, *n, *w, *iw;
{
  integer k, kk, t;

  switch (*typ) {
  case 1:
    CUPROD(integer1);
    break;
  case 2:
    CUPROD(integer2);
    break;
  case 4:
    CUPROD(integer);
    break;
  case 11:
    CUPROD(unsigned char);
    break;
  case 12:
    CUPROD(unsigned short);
    break;
  case 14:
    CUPROD(unsigned int);
    break;
  }
  return 0;
}

