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


#include "gendiag.h"
#include "core_math.h"

#define DIAG(Type) {\
Type *DX;\
Type *DY;\
    DX=(Type *)dx;\
    DY=(Type *)dy;\
    --DY;\
    for (i = 1; i <= nn*nn; ++i) {\
       DY[i] = 0;}\
    if (*k>=0) {\
      iy=1+(*k)*nn;}\
    else {\
      iy=1-(*k);}\
    for (i = 0; i < *n; ++i) {\
       DY[iy+i*(nn+1)] = DX[i];}\
 }

int C2F(gendiag)(int *typ, int *n, int *k, int *dx, int *dy)
{
    static int i, iy, nn;

    nn = *n + Abs(*k);

    switch (*typ)
    {
        case 1:
            DIAG(integer1);
            break;
        case 2:
            DIAG(integer2);
            break;
        case 4:
            DIAG(int) ;
            break;
        case 11:
            DIAG(unsigned char);
            break;
        case 12:
            DIAG(unsigned short);
            break;
        case 14:
            DIAG(unsigned int);
            break;
    }
    return 0;
}

