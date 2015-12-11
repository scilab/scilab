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

