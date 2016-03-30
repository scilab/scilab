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

#include "genisany.h"

#define ISANY(Type) {\
Type *DX;\
 DX=(Type *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) if ((int) DX[i]!=0) {dtemp = 1;break;}\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) if ((int) DX[i]!=0) {dtemp = 1;break;}\
 }\
}

int C2F(genisany)(int *typ, int *n, int *dx, int *incx)
{
    int  i1, i2;
    static int i, dtemp;


    dtemp = 0;
    if (*n <= 0)
    {
        return dtemp;
    }

    switch (*typ)
    {
        case 1:
            ISANY(integer1);
            break;
        case 2:
            ISANY(integer2);
            break;
        case 4:
            ISANY(int) ;
            break;
        case 11:
            ISANY(unsigned char);
            break;
        case 12:
            ISANY(unsigned short);
            break;
        case 14:
            ISANY(unsigned int);
            break;
    }
    return dtemp;
}


