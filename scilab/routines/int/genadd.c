#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define ADD(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= nv; ++i) \
	 DY[i] = DX[i] + DY[i];\
    else {\
      ix = 1; iy = 1;\
      if (*incx < 0) ix = (-nv + 1) * *incx + 1;\
      if (*incy < 0) iy = (-nv + 1) * *incy + 1;\
      for (i = 1; i <= nv; ++i) {\
	DY[iy] = DX[ix] + DY[iy];\
	ix += *incx; iy += *incy;\
      }\
    }\
}

int C2F(genadd1)(typ, n, dx, incx, dy, incy)
integer *n;
integer *incx;
integer *incy;
integer *typ;
int *dx;
int *dy;
{
  static integer i, ix, iy, nv;
  nv=*n;
  if (nv <= 0) return 0;


  switch (*typ) {
  case 1:
    ADD(integer1);
    break;
  case 2:
    ADD(integer2);
    break;
  case 4:
    ADD(integer);
    break;
  case 11:
    ADD(unsigned char);
    break;
  case 12:
    ADD(unsigned short);
    break;
  case 14:
    ADD(unsigned int);
    break;
  }
  return 0;
}

