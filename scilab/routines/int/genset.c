#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define SET(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return 0;\
    iy = 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= *n; ++i) { \
	DY[iy] = *DX;iy += *incy;}\
}




int C2F(genset)(typ, n, dx, dy, incy)
integer *n;
integer *incy;
integer *typ;
int *dx;
int *dy;
{
  static integer i, iy;
  switch (*typ) {
  case 0:
    SET(double);
    break;
  case 1:
    SET(integer1);
    break;
  case 2:
    SET(integer2);
    break;
  case 4:
    SET(integer);
    break;
  case 11:
    SET(unsigned char);
    break;
  case 12:
    SET(unsigned short);
    break;
  case 14:
    SET(unsigned int);
    break;
  }
  return 0;
}

