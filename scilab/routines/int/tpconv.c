#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define TCONV(Xtype,Ytype) {\
Xtype *DX;\
Ytype *DY;\
    DY=(Ytype *)dy;\
    --DY;\
    DX=(Xtype *)dx;\
    --DX;\
    if (*incx == 1 && *incy == 1) {\
      for (i = 1; i <= i1; ++i) DY[i] = (Ytype) DX[i];\
    }\
    else {\
      ix = 1;iy = 1;\
      if (*incx < 0) {ix = (-i1 + 1) * (*incx) + 1;}\
      if (*incy < 0) {iy = (-i1 + 1) * (*incy) + 1;}\
      for (i = 1; i <= i1; ++i) {\
	DY[iy] = (Ytype) DX[ix];\
	ix += *incx;iy += *incy;\
      }\
    }\
}

int C2F(tpconv)(xtyp, ytyp, n, dx, incx, dy, incy)
integer *n;
integer *incx;
integer *incy;
integer *xtyp, *ytyp;
void *dx;
void *dy;
{
  integer i1;
  static integer i, ix, iy;

  i1 = *n;
  if (i1 <= 0) return 0;

  switch (*xtyp) {
  case 0:
    switch (*ytyp) {
    case 0:
      TCONV(double,double);
      break;
    case 1:
      TCONV(double,integer1);
      break;
    case 2:
      TCONV(double,integer2);
      break;
    case 4:
      TCONV(double,integer);
      break;
    case 11:
      TCONV(double,unsigned char);
      break;
    case 12:
      TCONV(double,unsigned short);
      break;
    case 14:
      TCONV(double,unsigned int);
      break;
    }
    break;
  case 1:
    switch (*ytyp) {
    case 0:
      TCONV(integer1,double);
      break;
    case 1:
      TCONV(integer1,integer1);
      break;
    case 2:
      TCONV(integer1,integer2);
      break;
    case 4:
      TCONV(integer1,integer);
      break;
    case 11:
      TCONV(integer1,unsigned char);
      break;
    case 12:
      TCONV(integer1,unsigned short);
      break;
    case 14:
      TCONV(integer1,unsigned int);
      break;
    }
    break;
  case 2:
    switch (*ytyp) {
    case 0:
      TCONV(integer2,double);
      break;
    case 1:
      TCONV(integer2,integer1);
      break;
    case 2:
      TCONV(integer2,integer2);
      break;
    case 4:
      TCONV(integer2,integer);
      break;
    case 11:
      TCONV(integer2,unsigned char);
      break;
    case 12:
      TCONV(integer2,unsigned short);
      break;
    case 14:
      TCONV(integer2,unsigned int);
      break;
    }
    break;
  case 4:
    switch (*ytyp) {
    case 0:
      TCONV(integer,double);
      break;
    case 1:
      TCONV(integer,integer1);
      break;
    case 2:
      TCONV(integer,integer2);
      break;
    case 4:
      TCONV(integer,integer);
      break;
    case 11:
      TCONV(integer,unsigned char);
      break;
    case 12:
      TCONV(integer,unsigned short);
      break;
    case 14:
      TCONV(integer,unsigned int);
      break;
    }
    break;
  case 11:
    switch (*ytyp) {
    case 0:
      TCONV(unsigned char,double);
      break;
    case 1:
      TCONV(unsigned char,integer1);
      break;
    case 2:
      TCONV(unsigned char,integer2);
      break;
    case 4:
      TCONV(unsigned char,integer);
      break;
    case 11:
      TCONV(unsigned char,unsigned char);
      break;
    case 12:
      TCONV(unsigned char,unsigned short);
      break;
    case 14:
      TCONV(unsigned char,unsigned int);
      break;
    }
    break;
  case 12:
    switch (*ytyp) {
    case 0:
      TCONV(unsigned short,double);
      break;
    case 1:
      TCONV(unsigned short,integer1);
      break;
    case 2:
      TCONV(unsigned short,integer2);
      break;
    case 4:
      TCONV(unsigned short,integer);
      break;
    case 11:
      TCONV(unsigned short,unsigned char);
      break;
    case 12:
      TCONV(unsigned short,unsigned short);
      break;
    case 14:
      TCONV(unsigned short,unsigned int);
      break;
    }
    break;
  case 14:
   switch (*ytyp) {
    case 0:
      TCONV(unsigned int,double);
      break;
    case 1:
      TCONV(unsigned int,integer1);
      break;
    case 2:
      TCONV(unsigned int,integer2);
      break;
    case 4:
      TCONV(unsigned int,integer);
      break;
    case 11:
      TCONV(unsigned int,unsigned char);
      break;
    case 12:
      TCONV(unsigned int,unsigned short);
      break;
    case 14:
      TCONV(unsigned int,unsigned int);
      break;
   }
   break;
  }
  return 0;
}

