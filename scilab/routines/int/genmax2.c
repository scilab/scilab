#include "../machine.h"
typedef char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define MAX2(Xtype,Ytype) {\
Xtype *DX;\
Ytype *DY;\
    DY=(Ytype *)dy;\
    --DY;\
    DX=(Xtype *)dx;\
    --DX;\
    ix = 1;\
    for (i = 1; i <= *n; ++i) {\
      if ((Ytype)DX[ix] > DY[i]) {\
        DY[i] = (Ytype)DX[ix];\
	ind[i-1] = *no;}\
	ix = ix + *incx;\
    }\
}

int C2F(genmax2)(n, dx, incx, xtyp, dy, ytyp, ind, no)
integer *n, *no;
integer *xtyp, *ytyp;
integer *incx;
void *dx;
void *dy;
integer *ind;
{

  static integer i,ix;


  switch (*xtyp) {
  case 0:
    switch (*ytyp) {
    case 0:
      MAX2(double,double);
      break;
    }
    break;
  case 1:
    switch (*ytyp) {
    case 0:
      MAX2(integer1,double);
      break;
    case 1:
      MAX2(integer1,integer1);
      break;
    }
    break;
  case 2:
    switch (*ytyp) {
    case 0:
      MAX2(integer2,double);
      break;
    case 2:
      MAX2(integer2,integer2);
      break;
    case 4:
      MAX2(integer2,integer);
      break;
    case 12:
      MAX2(integer2,unsigned short);
      break;
    case 14:
      MAX2(integer2,unsigned int);
      break;
    }
    break;
  case 4:
    switch (*ytyp) {
    case 0:
      MAX2(integer,double);
      break;
    case 4:
      MAX2(integer,integer);
      break;
    case 14:
      MAX2(integer,unsigned int);
      break;
    }
    break;
  }
}

