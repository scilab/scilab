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

#include "genscal.h"

#define SCAL(Type) {\
Type *DX;\
Type *DA;\
    DX=(Type *)dx;\
    --DX;\
    DA=(Type *)da;\
    for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) {\
	DX[i] = (*DA) * DX[i];\
    }\
}

int C2F(genscal)(int *typ, int *n, int *da, int *dx, int *incx)
{
    static int i, i1, i2, nincx;
    if (*n <= 0 || *incx <= 0)
    {
        return 0;
    }
    nincx = *n * (*incx);
    i1 = nincx;
    i2 = *incx;

    switch (*typ)
    {
        case 1:
            SCAL(integer1);
            break;
        case 2:
            SCAL(integer2);
            break;
        case 4:
            SCAL(int) ;
            break;
        case 11:
            SCAL(unsigned char);
            break;
        case 12:
            SCAL(unsigned short);
            break;
        case 14:
            SCAL(unsigned int);
            break;
    }
    return 0;
}

