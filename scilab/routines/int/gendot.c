#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define DOT(Type) {\
Type *DX;\
Type *DY;\
Type C;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return (Type)itemp;\
    ix = 1;iy = 1;\
    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= i1; ++i) {\
	itemp += DX[ix] * DY[iy];\
	ix += *incx;\
	iy += *incy;\
    }\
    C = (Type) itemp;\
    return C;\
}

int C2F(gendot)(typ,n, dx, incx, dy, incy)
integer *n, *incx, *incy;
integer *typ;
int *dx;
int *dy;
{
  integer i1;
  static integer i, itemp, ix, iy;

  itemp = 0;
  i1 = *n;

  switch (*typ) {
  case 1:
    DOT(integer1);
    break;
  case 2:
    DOT(integer2);
    break;
  case 4:
    DOT(integer);
    break;
  case 11:
    DOT(unsigned char);
    break;
  case 12:
    DOT(unsigned short);
    break;
  case 14:
    DOT(unsigned int);
    break;
  }
  return 0;
}

