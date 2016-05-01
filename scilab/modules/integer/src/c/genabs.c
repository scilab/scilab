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

#include "genabs.h"
#include "core_math.h"

#define ABS(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    --DX;\
    for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) {\
	DX[i] = Abs(DX[i]);\
    }\
}

int C2F(genabs)(int *typ, int *n, int *dx, int *incx)
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
            ABS(integer1);
            break;
        case 2:
            ABS(integer2);
            break;
        case 4:
            ABS(int) ;
            break;
        case 11:
            ABS(unsigned char);
            break;
        case 12:
            ABS(unsigned short);
            break;
        case 14:
            ABS(unsigned int);
            break;
    }
    return 1; /* Warning missing return statement at end of non-void */
}

