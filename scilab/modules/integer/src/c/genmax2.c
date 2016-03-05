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

#include "genmax2.h"

#define MAX2(Xtype,Ytype) {\
Xtype *DX;\
Ytype *DY;\
    DY=(Ytype *)dy;\
    --DY;\
    DX=(Xtype *)dx;\
    --DX;\
    ix = 1;\
    for (i = 1; i <= *n; ++i) {\
      if ((Ytype)DX[ix] > DY[i]) {\
        DY[i] = (Ytype)DX[ix];\
	ind[i-1] = *no;}\
	ix = ix + *incx;\
    }\
}

void C2F(genmax2)(int *n, void *dx, int *incx, int *xtyp, void *dy, int *ytyp, int *ind, int *no)
{

    static int i, ix;
    /*  printf("%d %d\n",*xtyp,*ytyp);*/

    switch (*xtyp)
    {
        case 0:
            switch (*ytyp)
            {
                case 0:
                    MAX2(double, double);
                    break;
                case 4:
                    MAX2(double, integer);
                    break;
                case 2:
                    MAX2(double, integer2);
                    break;
                case 1:
                    MAX2(double, integer1);
                    break;
                case 14:
                    MAX2(double, unsigned int);
                    break;
                case 12:
                    MAX2(double, unsigned short);
                    break;
                case 11:
                    MAX2(double, unsigned char);
                    break;
            }
            break;
        case 1:
            switch (*ytyp)
            {
                case 0:
                    MAX2(integer1, double);
                    break;
                case 4:
                    MAX2(integer1, integer);
                    break;
                case 2:
                    MAX2(integer1, integer2);
                    break;
                case 1:
                    MAX2(integer1, integer1);
                    break;
                case 14:
                    MAX2(integer1, unsigned int);
                    break;
                case 12:
                    MAX2(integer1, unsigned short);
                    break;
                case 11:
                    MAX2(integer1, unsigned char);
                    break;
            }
            break;
        case 2:
            switch (*ytyp)
            {
                case 0:
                    MAX2(integer2, double);
                    break;
                case 4:
                    MAX2(integer2, integer);
                    break;
                case 2:
                    MAX2(integer2, integer2);
                    break;
                case 1:
                    MAX2(integer2, integer1);
                    break;
                case 14:
                    MAX2(integer2, unsigned int);
                    break;
                case 12:
                    MAX2(integer2, unsigned short);
                    break;
                case 11:
                    MAX2(integer2, unsigned char);
                    break;
            }
            break;
        case 4:
            switch (*ytyp)
            {
                case 0:
                    MAX2(integer, double);
                    break;
                case 2:
                    MAX2(integer, integer2);
                    break;
                case 4:
                    MAX2(integer, integer);
                    break;
                case 11:
                    MAX2(integer, unsigned char);
                    break;
                case 12:
                    MAX2(integer, unsigned short);
                    break;
                case 14:
                    MAX2(integer, unsigned int);
                    break;
            }
            break;
        case 11:
            switch (*ytyp)
            {
                case 0:
                    MAX2(unsigned char, double);
                    break;
                case 4:
                    MAX2(unsigned char, integer);
                    break;
                case 2:
                    MAX2(unsigned char, integer2);
                    break;
                case 1:
                    MAX2(unsigned char, integer1);
                    break;
                case 14:
                    MAX2(unsigned char, unsigned int);
                    break;
                case 12:
                    MAX2(unsigned char, unsigned short);
                    break;
                case 11:
                    MAX2(unsigned char, unsigned char);
                    break;
            }
            break;
        case 12:
            switch (*ytyp)
            {
                case 0:
                    MAX2(unsigned short, double);
                    break;
                case 4:
                    MAX2(unsigned short, integer);
                    break;
                case 2:
                    MAX2(unsigned short, integer2);
                    break;
                case 1:
                    MAX2(unsigned short, integer1);
                    break;
                case 14:
                    MAX2(unsigned short, unsigned int);
                    break;
                case 12:
                    MAX2(unsigned short, unsigned short);
                    break;
                case 11:
                    MAX2(unsigned short, unsigned char);
                    break;
            }
            break;
        case 14:
            switch (*ytyp)
            {
                case 0:
                    MAX2(integer, double);
                    break;
                case 4:
                    MAX2(integer, integer);
                    break;
                case 2:
                    MAX2(integer, integer2);
                    break;
                case 1:
                    MAX2(integer, integer1);
                    break;
                case 14:
                    MAX2(integer, unsigned int);
                    break;
                case 12:
                    MAX2(integer, unsigned short);
                    break;
                case 11:
                    MAX2(integer, unsigned char);
                    break;
            }
            break;
    }
}

