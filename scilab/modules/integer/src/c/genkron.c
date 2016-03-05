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

#include "genkron.h"

#define KRON(Type) {\
Type *DX;\
Type *DY;\
Type *DR;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    --DY;\
    DR=(Type *)dr;\
    --DR;\
    kx1 = 1 - ix;\
    kk1 = -*ny;\
    for (jx = 1; jx <= *nx; ++jx) {\
      ky = 1;\
      kx1 = kx1 + ix;\
      kk1 = kk1 + *ny;\
      for (jy = 1; jy <= *ny; ++jy){\
        kx = kx1;\
        kk = 1 + (jy - 1 + kk1) * ir;\
        for (i = 1; i <= *mx; ++i){\
          for (l = 0; l < *my; ++l)\
   	       DR[kk + l] = DY[ky + l] * DX[kx];\
   	     kk = kk + *my;\
   	     kx = kx + 1;}\
   	   ky = ky + iy;}}\
}


int C2F(genkron)(int *typ, int *dx, int *incx, int *mx, int *nx, int *dy, int *incy, int *my, int *ny, int *dr, int *incr)
{
    static int i, ix, iy, ir, jx, jy, l, kk1, kx1, kk, kx, ky;
    ix = *incx;
    iy = *incy;
    ir = *incr;

    switch (*typ)
    {
        case 1:
            KRON(integer1);
            break;
        case 2:
            KRON(integer2);
            break;
        case 4:
            KRON(int) ;
            break;
        case 11:
            KRON(unsigned char);
            break;
        case 12:
            KRON(unsigned short);
            break;
        case 14:
            KRON(unsigned int);
            break;
    }
    return 0;
}

