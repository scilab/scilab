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

#include "genprod.h"

#define PROD(Typein,Typeout) {\
Typein *DX;\
 DX=(Typein *)dx;\
 --DX;\
 if (*incx == 1) {\
   for (i = 1; i <= *n; ++i) dtemp *= (Typeout) DX[i];\
 }\
 else {\
   i1 = *n * *incx;i2 = *incx;\
   for (i = 1; i2 < 0 ? i >= i1 : i <= i1; i += i2) dtemp *= (Typeout) DX[i];\
 }\
}

int C2F(genprod)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i, dtemp;


    dtemp = 1;
    if (*n <= 0)
    {
        return dtemp;
    }

    switch (*typ)
    {
        case 1:
            PROD(integer1, int);
            break;
        case 2:
            PROD(integer2, int);
            break;
        case 4:
            PROD(int, int) ;
            break;
        case 11:
            PROD(unsigned char, int);
            break;
        case 12:
            PROD(unsigned short, int);
            break;
        case 14:
            PROD(unsigned int, int);
            break;
    }
    return dtemp;
}

double C2F(genprodasdouble)(int *typ, int *n, void *dx, int *incx)
{
    int  i1, i2;
    static int i;
    static double dtemp;


    dtemp = 1.0;
    if (*n <= 0)
    {
        return dtemp;
    }

    switch (*typ)
    {
        case 1:
            PROD(integer1, double);
            break;
        case 2:
            PROD(integer2, double);
            break;
        case 4:
            PROD(int, double);
            break;
        case 11:
            PROD(unsigned char, double);
            break;
        case 12:
            PROD(unsigned short, double);
            break;
        case 14:
            PROD(unsigned int, double);
            break;
    }
    return dtemp;
}


