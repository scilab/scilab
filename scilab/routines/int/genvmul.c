#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define VMUL(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1) {\
      for (i = 1; i <= i1; ++i) DY[i] *= DX[i];\
    }\
    else {\
    ix = 1;iy = 1;\
    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= i1; ++i) {\
	DY[iy] *= DX[ix];\
	ix += *incx;iy += *incy;\
    }\
    }\
}


int C2F(genvmul)(typ,n, dx, incx, dy, incy)
integer *typ;
integer *n;
integer1 *dx;
integer *incx;
integer1 *dy;
integer *incy;
{
  integer i1;
  static integer i, ix, iy;

  i1 = *n;

  if (i1 <= 0) {
    return 0;
  }
  switch (*typ) {
  case 1:
    VMUL(integer1);
    break;
  case 2:
    VMUL(integer2);
    break;
  case 4:
    VMUL(integer);
    break;
  case 11:
    VMUL(unsigned char);
    break;
  case 12:
    VMUL(unsigned short);
    break;
  case 14:
    VMUL(unsigned int);
    break;
  }
  return 0;
}
