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

#include "gendot.h"

#define DOT(Type) {\
Type *DX;\
Type *DY;\
Type C;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return (Type)itemp;\
    ix = 1;iy = 1;\
    if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= i1; ++i) {\
	itemp += DX[ix] * DY[iy];\
	ix += *incx;\
	iy += *incy;\
    }\
    C = (Type) itemp;\
    return C;\
}

int C2F(gendot)(int *typ, int *n, void *dx, int *incx, void *dy, int *incy)
{
    int i1;
    static int i, itemp, ix, iy;

    itemp = 0;
    i1 = *n;

    switch (*typ)
    {
        case 1:
            DOT(integer1);
            break;
        case 2:
            DOT(integer2);
            break;
        case 4:
            DOT(int) ;
            break;
        case 11:
            DOT(unsigned char);
            break;
        case 12:
            DOT(unsigned short);
            break;
        case 14:
            DOT(unsigned int);
            break;
    }
    return 0;
}

