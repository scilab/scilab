#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define CONV(Type) {\
Type *DY;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1) \
      for (i = 1; i <= i1; ++i) DY[i] = (Type) dx[i];\
    else {\
      ix = 1;iy = 1;\
      if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
      if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
      for (i = 1; i <= i1; ++i) {\
	DY[iy] = (Type) dx[ix];\
	ix += *incx;iy += *incy;\
      }\
   }\
}

int C2F(db2int)(typ, n, dx, incx, dy, incy)
integer *n;
integer *incx;
integer *incy;
integer *typ;
double *dx;
int *dy;
{
  integer i1;
  static integer i, ix, iy;

  --dx;
  i1 = *n;
  if (i1 <= 0) return 0;
  switch (*typ) {
  case 1:
    CONV(integer1);
    break;
  case 2:
    CONV(integer2);
    break;
  case 4:
    CONV(integer);
    break;
  }
}

