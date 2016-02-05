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

#include "genadd.h"

#define ADD(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= nv; ++i) \
	 DY[i] = DX[i] + DY[i];\
    else {\
      ix = 1; iy = 1;\
      if (*incx < 0) ix = (-nv + 1) * *incx + 1;\
      if (*incy < 0) iy = (-nv + 1) * *incy + 1;\
      for (i = 1; i <= nv; ++i) {\
	DY[iy] = DX[ix] + DY[iy];\
	ix += *incx; iy += *incy;\
      }\
    }\
}

int C2F(genadd1)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy)
{
    static int i, ix, iy, nv;
    nv = *n;
    if (nv <= 0)
    {
        return 0;
    }


    switch (*typ)
    {
        case 1:
            ADD(integer1);
            break;
        case 2:
            ADD(integer2);
            break;
        case 4:
            ADD(int) ;
            break;
        case 11:
            ADD(unsigned char);
            break;
        case 12:
            ADD(unsigned short);
            break;
        case 14:
            ADD(unsigned int);
            break;
    }
    return 0;
}

