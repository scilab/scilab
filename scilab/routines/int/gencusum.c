#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define CUSUM(Type) {\
Type *W;\
    W=(Type *)w;\
    t = 0; kk = 0;\
    for (k = 0; k < *n; ++k) {\
        t += (integer)W[kk];\
	W[kk] =(Type)t;kk += (*iw);\
    }\
}


int C2F(gencusum)(typ,n, w, iw)
integer *typ, *n, *w, *iw;
{
  integer k, kk, t;

  switch (*typ) {
  case 1:
    CUSUM(integer1);
    break;
  case 2:
    CUSUM(integer2);
    break;
  case 4:
    CUSUM(integer);
    break;
  case 11:
    CUSUM(unsigned char);
    break;
  case 12:
    CUSUM(unsigned short);
    break;
  case 14:
    CUSUM(unsigned int);
    break;
  }
  return 0;
}

