#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define SCAL(Type) {\
Type *DX;\
Type *DA;\
    DX=(Type *)dx;\
    --DX;\
    DA=(Type *)da;\
    for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) {\
	DX[i] = (*DA) * DX[i];\
    }\
}

int C2F(genscal)(typ, n, da, dx, incx)
integer *n;
integer *incx;
integer *typ;
int *da;
int *dx;
{
  static integer i, i1, i2, nincx;
  if (*n <= 0 || *incx <= 0) return 0;
  nincx = *n * (*incx);
  i1 = nincx;
  i2 = *incx;

  switch (*typ) {
  case 1:
    SCAL(integer1);
    break;
  case 2:
    SCAL(integer2);
    break;
  case 4:
    SCAL(integer);
    break;
  case 11:
    SCAL(unsigned char);
    break;
  case 12:
    SCAL(unsigned short);
    break;
  case 14:
    SCAL(unsigned int);
    break;
  }
  return 0;
}

