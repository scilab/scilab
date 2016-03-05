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

#include "tpconv.h"


#define TCONV(Xtype,Ytype) {\
Xtype *DX;\
Ytype *DY;\
    DY=(Ytype *)dy;\
    --DY;\
    DX=(Xtype *)dx;\
    --DX;\
    if (*incx == 1 && *incy == 1) {\
      for (i = 1; i <= i1; ++i) DY[i] = (Ytype) DX[i];\
    }\
    else {\
      ix = 1;iy = 1;\
      if (*incx < 0) {ix = (-i1 + 1) * (*incx) + 1;}\
      if (*incy < 0) {iy = (-i1 + 1) * (*incy) + 1;}\
      for (i = 1; i <= i1; ++i) {\
	DY[iy] = (Ytype) DX[ix];\
	ix += *incx;iy += *incy;\
      }\
    }\
}

int C2F(tpconv)(int *xtyp, int *ytyp, int *n, void *dx, int *incx, void *dy, int *incy)
{
    int i1;
    static int i, ix, iy;

    i1 = *n;
    if (i1 <= 0)
    {
        return 0;
    }

    switch (*xtyp)
    {
        case 0:
            switch (*ytyp)
            {
                case 0:
                    TCONV(double, double);
                    break;
                case 1:
                    TCONV(double, integer1);
                    break;
                case 2:
                    TCONV(double, integer2);
                    break;
                case 4:
                    TCONV(double, integer);
                    break;
                case 8:
                    TCONV(double, long long);
                    break;
                case 11:
                    TCONV(double, unsigned char);
                    break;
                case 12:
                    TCONV(double, unsigned short);
                    break;
                case 14:
                    TCONV(double, unsigned int);
                    break;
                case 18:
                    TCONV(double, unsigned long long);
                    break;
            }
            break;
        case 1:
            switch (*ytyp)
            {
                case 0:
                    TCONV(integer1, double);
                    break;
                case 1:
                    TCONV(integer1, integer1);
                    break;
                case 2:
                    TCONV(integer1, integer2);
                    break;
                case 4:
                    TCONV(integer1, integer);
                    break;
                case 8:
                    TCONV(integer1, long long);
                    break;
                case 11:
                    TCONV(integer1, unsigned char);
                    break;
                case 12:
                    TCONV(integer1, unsigned short);
                    break;
                case 14:
                    TCONV(integer1, unsigned int);
                    break;
                case 18:
                    TCONV(integer1, unsigned long long);
                    break;
            }
            break;
        case 2:
            switch (*ytyp)
            {
                case 0:
                    TCONV(integer2, double);
                    break;
                case 1:
                    TCONV(integer2, integer1);
                    break;
                case 2:
                    TCONV(integer2, integer2);
                    break;
                case 4:
                    TCONV(integer2, integer);
                    break;
                case 8:
                    TCONV(integer2, long long);
                    break;
                case 11:
                    TCONV(integer2, unsigned char);
                    break;
                case 12:
                    TCONV(integer2, unsigned short);
                    break;
                case 14:
                    TCONV(integer2, unsigned int);
                    break;
                case 18:
                    TCONV(integer2, unsigned long long);
                    break;
            }
            break;
        case 4:
            switch (*ytyp)
            {
                case 0:
                    TCONV(integer, double);
                    break;
                case 1:
                    TCONV(integer, integer1);
                    break;
                case 2:
                    TCONV(integer, integer2);
                    break;
                case 4:
                    TCONV(integer, integer);
                    break;
                case 8:
                    TCONV(integer, long long);
                    break;
                case 11:
                    TCONV(integer, unsigned char);
                    break;
                case 12:
                    TCONV(integer, unsigned short);
                    break;
                case 14:
                    TCONV(integer, unsigned int);
                    break;
                case 18:
                    TCONV(integer, unsigned long long);
                    break;
            }
            break;
        case 8:
            switch (*ytyp)
            {
                case 0:
                    TCONV(long long, double);
                    break;
                case 1:
                    TCONV(long long, integer1);
                    break;
                case 2:
                    TCONV(long long, integer2);
                    break;
                case 4:
                    TCONV(long long, integer);
                    break;
                case 8:
                    TCONV(long long, long long);
                    break;
                case 11:
                    TCONV(long long, unsigned char);
                    break;
                case 12:
                    TCONV(long long, unsigned short);
                    break;
                case 14:
                    TCONV(long long, unsigned int);
                    break;
                case 18:
                    TCONV(long long, unsigned long long);
                    break;
            }
            break;
        case 11:
            switch (*ytyp)
            {
                case 0:
                    TCONV(unsigned char, double);
                    break;
                case 1:
                    TCONV(unsigned char, integer1);
                    break;
                case 2:
                    TCONV(unsigned char, integer2);
                    break;
                case 4:
                    TCONV(unsigned char, integer);
                    break;
                case 8:
                    TCONV(unsigned char, long long);
                    break;
                case 11:
                    TCONV(unsigned char, unsigned char);
                    break;
                case 12:
                    TCONV(unsigned char, unsigned short);
                    break;
                case 14:
                    TCONV(unsigned char, unsigned int);
                    break;
                case 18:
                    TCONV(unsigned char, unsigned long long);
                    break;
            }
            break;
        case 12:
            switch (*ytyp)
            {
                case 0:
                    TCONV(unsigned short, double);
                    break;
                case 1:
                    TCONV(unsigned short, integer1);
                    break;
                case 2:
                    TCONV(unsigned short, integer2);
                    break;
                case 4:
                    TCONV(unsigned short, integer);
                    break;
                case 8:
                    TCONV(unsigned short, long long);
                    break;
                case 11:
                    TCONV(unsigned short, unsigned char);
                    break;
                case 12:
                    TCONV(unsigned short, unsigned short);
                    break;
                case 14:
                    TCONV(unsigned short, unsigned int);
                    break;
                case 18:
                    TCONV(unsigned short, unsigned long long);
                    break;
            }
            break;
        case 14:
            switch (*ytyp)
            {
                case 0:
                    TCONV(unsigned int, double);
                    break;
                case 1:
                    TCONV(unsigned int, integer1);
                    break;
                case 2:
                    TCONV(unsigned int, integer2);
                    break;
                case 4:
                    TCONV(unsigned int, integer);
                    break;
                case 8:
                    TCONV(unsigned int, long long);
                    break;
                case 11:
                    TCONV(unsigned int, unsigned char);
                    break;
                case 12:
                    TCONV(unsigned int, unsigned short);
                    break;
                case 14:
                    TCONV(unsigned int, unsigned int);
                    break;
                case 18:
                    TCONV(unsigned int, unsigned long long);
                    break;
            }
            break;
        case 18:
            switch (*ytyp)
            {
                case 0:
                    TCONV(unsigned long long, double);
                    break;
                case 1:
                    TCONV(unsigned long long, integer1);
                    break;
                case 2:
                    TCONV(unsigned long long, integer2);
                    break;
                case 4:
                    TCONV(unsigned long long, integer);
                    break;
                case 8:
                    TCONV(unsigned long long, long long);
                    break;
                case 11:
                    TCONV(unsigned long long, unsigned char);
                    break;
                case 12:
                    TCONV(unsigned long long, unsigned short);
                    break;
                case 14:
                    TCONV(unsigned long long, unsigned int);
                    break;
                case 18:
                    TCONV(unsigned long long, unsigned long long);
                    break;
            }
            break;
    }
    return 0;
}

