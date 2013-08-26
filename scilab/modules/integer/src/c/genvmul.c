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

#include "genvmul.h"

#define VMUL(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*incx == 1 && *incy == 1) {\
      for (i = 1; i <= i1; ++i) DY[i] *= DX[i];\
    }\
    else {\
    ix = 1;iy = 1;\
    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= i1; ++i) {\
	DY[iy] *= DX[ix];\
	ix += *incx;iy += *incy;\
    }\
    }\
}


int C2F(genvmul)(int *typ, int *n, integer1 *dx, int *incx, integer1 *dy, int *incy)
{
    int i1;
    static int i, ix, iy;

    i1 = *n;

    if (i1 <= 0)
    {
        return 0;
    }
    switch (*typ)
    {
        case 1:
            VMUL(integer1);
            break;
        case 2:
            VMUL(integer2);
            break;
        case 4:
            VMUL(int) ;
            break;
        case 11:
            VMUL(unsigned char);
            break;
        case 12:
            VMUL(unsigned short);
            break;
        case 14:
            VMUL(unsigned int);
            break;
    }
    return 0;
}
