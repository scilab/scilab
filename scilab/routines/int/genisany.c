#include "../machine.h"
typedef signed char integer1;
typedef short integer2;
/*     Copyright INRIA */

#define ISANY(Type) {\
Type *DX;\
 DX=(Type *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) if ((integer)DX[i]!=0) {dtemp = 1;break;}\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) if ((integer)DX[i]!=0) {dtemp = 1;break;}\
 }\
}

integer C2F(genisany)(typ, n, dx, incx)
integer *typ,*n, *dx, *incx;
{
    integer  i1, i2;
    static integer i, dtemp;


    dtemp = 0;
    if (*n <= 0) return dtemp;

    switch (*typ) {
    case 1:
      ISANY(integer1);
      break;
    case 2:
      ISANY(integer2);
      break;
    case 4:
      ISANY(integer);
      break;
    case 11:
      ISANY(unsigned char);
      break;
    case 12:
      ISANY(unsigned short);
      break;
    case 14:
      ISANY(unsigned int);
      break;
    }
    return dtemp;
}


