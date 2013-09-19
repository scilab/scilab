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

#include "db2int.h"

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

int C2F(db2int)(int *typ, int *n, double *dx, int *incx, int *dy, int *incy)
{
    int i1;
    static int i, ix, iy;

    --dx;
    i1 = *n;
    if (i1 <= 0)
    {
        return 0;
    }
    switch (*typ)
    {
        case 1:
            CONV(integer1);
            break;
        case 2:
            CONV(integer2);
            break;
        case 4:
            CONV(int) ;
            break;
    }
    return 0;
}

