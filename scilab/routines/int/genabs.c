#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */
#define Abs(x) ( ( (x) >= 0) ? (x) : -( x) )

#define ABS(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    --DX;\
    for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) {\
	DX[i] = Abs(DX[i]);\
    }\
}

int C2F(genabs)(typ, n, dx, incx)
integer *n;
integer *incx;
integer *typ;
int *dx;
{
  static integer i, i1, i2, nincx;
  if (*n <= 0 || *incx <= 0) return 0;
  nincx = *n * (*incx);
  i1 = nincx;
  i2 = *incx;

  switch (*typ) {
  case 1:
    ABS(integer1);
    break;
  case 2:
    ABS(integer2);
    break;
  case 4:
    ABS(integer);
    break;
  case 11:
    ABS(unsigned char);
    break;
  case 12:
    ABS(unsigned short);
    break;
  case 14:
    ABS(unsigned int);
    break;
  }
  return 1; /* Warning missing return statement at end of non-void */
}

