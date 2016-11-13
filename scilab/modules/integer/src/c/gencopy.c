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


#include "gencopy.h"

#define COPYINT(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    if (*n <= 0) return 0;\
    if (*incx == 1 && *incy == 1)\
       for (i = 1; i <= *n; ++i) \
	 DY[i] = DX[i];\
    else {\
      ix = 1;\
      iy = 1;\
      if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;\
      if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;\
      for (i = 1; i <= *n; ++i) {\
	DY[iy] = DX[ix];\
	ix += *incx;\
	iy += *incy;\
      }\
	 };}



int C2F(gencopy)(int *typ, int *n, int *dx, int *incx, int *dy, int *incy)
{
    static int i, ix, iy;
    switch (*typ)
    {
        case 1:
            COPYINT(integer1);
            break;
        case 2:
            COPYINT(integer2);
            break;
        case 4:
            COPYINT(int) ;
            break;
        case 11:
            COPYINT(unsigned char);
            break;
        case 12:
            COPYINT(unsigned short);
            break;
        case 14:
            COPYINT(unsigned int);
            break;
    }
    return 0;
}

