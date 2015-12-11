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

