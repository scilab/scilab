#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define KRON(Type) {\
Type *DX;\
Type *DY;\
Type *DR;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    DR=(Type *)dr;\
    --DR;\
    kx1 = 1 - ix;\
    kk1 = -*ny;\
    for (jx = 1; jx <= *nx; ++jx) {\
      ky = 1;\
      kx1 = kx1 + ix;\
      kk1 = kk1 + *ny;\
      for (jy = 1; jy <= *ny; ++jy){\
        kx = kx1;\
        kk = 1 + (jy - 1 + kk1) * ir;\
        for (i = 1; i <= *mx; ++i){\
          for (l = 0; l < *my; ++l)\
   	       DR[kk + l] = DY[ky + l] * DX[kx];\
   	     kk = kk + *my;\
   	     kx = kx + 1;}\
   	   ky = ky + iy;}}\
}


int C2F(genkron)(typ,dx,incx,mx,nx,dy,incy,my,ny,dr,incr)
integer *incx, *mx, *nx;
integer *incy, *my, *ny;
integer *incr;
integer *typ;
int *dx;
int *dy;
int *dr;
{
  static integer i, ix, iy, ir, jx, jy, l, kk1, kx1, kk, kx, ky;
  ix = *incx;
  iy = *incy;
  ir = *incr;

  switch (*typ) {
  case 1:
    KRON(integer1);
    break;
  case 2:
    KRON(integer2);
    break;
  case 4:
    KRON(integer);
    break;
  case 11:
    KRON(unsigned char);
    break;
  case 12:
    KRON(unsigned short);
    break;
  case 14:
    KRON(unsigned int);
    break;
  }
  return 0;
}

