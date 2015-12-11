/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include "gensubst.h"

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



int C2F(gensubst)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy)
{
    static int i, ix, iy, i1;

    i1 = *n;
    if (i1 <= 0)
    {
        return 0;
    }

    switch (*typ)
    {
        case 1:
            SUBST(integer1);
            break;
        case 2:
            SUBST(integer2);
            break;
        case 4:
            SUBST(int) ;
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

#define SUBST1(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1){\
       for (i = 1; i <= i1; ++i)  DX[i] = DY[i] - DX[i];\
    }\
    else {\
      ix = 1; iy = 1;\
      if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
      if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
      for (i = 1; i <= i1; ++i) {\
	DX[ix] = DY[iy] - DX[ix];\
	ix += *incx; iy += *incy;\
      }\
 }\
}



int C2F(gensubst1)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy)
{
    static int i, ix, iy, i1;
    i1 = *n;
    if (i1 <= 0)
    {
        return 0;
    }

    switch (*typ)
    {
        case 1:
            SUBST1(integer1);
            break;
        case 2:
            SUBST1(integer2);
            break;
        case 4:
            SUBST1(int) ;
            break;
        case 11:
            SUBST1(unsigned char);
            break;
        case 12:
            SUBST1(unsigned short);
            break;
        case 14:
            SUBST1(unsigned int);
            break;
    }
    return 0;
}

