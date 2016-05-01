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

#include "genset.h"

#define SET(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return 0;\
    iy = 1;\
    if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
    for (i = 1; i <= *n; ++i) { \
	DY[iy] = *DX;iy += *incy;}\
}




int C2F(genset)(int *typ, int *n, int *dx, int *dy, int *incy)
{
    static int i, iy;
    switch (*typ)
    {
        case 0:
            SET(double);
            break;
        case 1:
            SET(integer1);
            break;
        case 2:
            SET(integer2);
            break;
        case 4:
            SET(int) ;
            break;
        case 11:
            SET(unsigned char);
            break;
        case 12:
            SET(unsigned short);
            break;
        case 14:
            SET(unsigned int);
            break;
    }
    return 0;
}

