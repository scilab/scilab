/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA -
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

