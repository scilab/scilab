#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define MMIN(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
      if (*job==2) {\
        for (l = 1; l <= *m; ++l){\
          DY[l] = DX[l];DK[l] = 1;\
	  for (k = 1; k < *n; ++k) {\
            if (DX[l+k*ix] < DY[l]) {\
              DY[l] = DX[l+k*ix];DK[l] = k+1;}}}}\
      if (*job==1) {\
        for (k = 0; k < *n; ++k){\
          kk = k*ix;\
          DY[1+k*iy] = DX[1+kk];DK[1+k*iy] = 1;\
	  for (l = 2; l <= *m; ++l) {\
            if (DX[l+kk] < DY[1+k*iy]) {\
              DY[1+k*iy] = DX[l+kk];DK[1+k*iy] = l;}}}}\
      if (*job==0) {\
        DY[1] = DX[1];DK[1] = 1;\
        for (k = 0; k < *n; ++k){\
          kk = k*ix;\
	  for (l = 1; l <= *m; ++l) {\
            if (DX[l+kk] < DY[1]) {\
	      DY[1] = DX[l+kk];DK[1] = l + kk;}}}}\
}


int C2F(genmmin)(typ, job, m, n, dx, incx, dy, dk, incy)
integer *m, *n;
integer *incx;
integer *incy;
integer *typ;
integer *job;
int *dx;
int *dy;
int *dk;
{
  static integer ix, iy, l, k, kk, *DK;

  DK=--dk;
  ix = *incx;
  iy = *incy;

  switch (*typ) {
  case 1:
    MMIN(integer1);
    break;
  case 2:
    MMIN(integer2);
    break;
  case 4:
    MMIN(integer);
    break;
  case 11:
    MMIN(unsigned char);
    break;
  case 12:
    MMIN(unsigned short);
    break;
  case 14:
    MMIN(unsigned int);
    break;
  }
  return 0;
}

