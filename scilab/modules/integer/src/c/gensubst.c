#include "machine.h"
#include "gensubst.h"
/*     Copyright INRIA */

#define SUBST(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1){\
       for (i = 1; i <= i1; ++i)  DX[i] = DX[i] - DY[i];\
    }\
    else {\
      ix = 1; iy = 1;\
      if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
      if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
      for (i = 1; i <= i1; ++i) {\
	DX[ix] = DX[ix] - DY[iy];\
	ix += *incx; iy += *incy;\
      }\
 }\
}



int C2F(gensubst)(integer *typ, integer *n, int *dx, integer *incx, int *dy, integer *incy)
{
  static integer i, ix, iy, i1;

  i1 = *n;
  if (i1 <= 0) return 0;

  switch (*typ) {
  case 1:
    SUBST(integer1);
    break;
  case 2:
    SUBST(integer2);
    break;
  case 4:
    SUBST(integer);
    break;
  case 11:
    SUBST(unsigned char);
    break;
  case 12:
    SUBST(unsigned short);
    break;
  case 14:
    SUBST(unsigned int);
    break;
  }
  return 0;
}

