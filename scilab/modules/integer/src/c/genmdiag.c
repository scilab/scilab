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

#include "genmdiag.h"
#include "core_math.h"

#define MDIAG(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    --DX;\
    DY=(Type *)dy;\
    if (*k>=0)\
      ix=1+(*k)*(*m);\
    else\
      ix=1-(*k);\
    for (i = 0; i < mn; ++i) \
       DY[i] = DX[ix+i*(*m+1)];\
 }


int C2F(genmdiag)(int *typ, int *m, int *n, int *k, int *dx, int *dy)
{
    static int i, ix, mn;

    if (*k >= 0)
    {
        mn = Max(0, Min(*m, *n - *k));
    }
    else
    {
        mn = Max(0, Min(*m + *k, *n));
    }

    switch (*typ)
    {
        case 1:
            MDIAG(integer1);
            break;
        case 2:
            MDIAG(integer2);
            break;
        case 4:
            MDIAG(int) ;
            break;
        case 11:
            MDIAG(unsigned char);
            break;
        case 12:
            MDIAG(unsigned short);
            break;
        case 14:
            MDIAG(unsigned int);
            break;
    }
    return 0;
}

