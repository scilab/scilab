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

#include "gentril.h"


#define TRIL(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>=0) {\
     l = l + (*m)*(kk+1);\
     nn = nn - kk - 1;\
     kk = -1;\
    }\
    ls = l;\
    ll = -kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > *m) ll = *m;\
      for (i = 0; i < ll; ++i) \
        DX[ls+i] = 0;\
      ll++;\
      ls = ls + *m;\
    }\
 }

int C2F(gentril)(int *typ, int *m, int *n, int *k, int *dx)
{
    static int kk, l, ls, ll, j, i, nn;

    switch (*typ)
    {
        case 1:
            TRIL(integer1);
            break;
        case 2:
            TRIL(integer2);
            break;
        case 4:
            TRIL(int) ;
            break;
        case 11:
            TRIL(unsigned char);
            break;
        case 12:
            TRIL(unsigned short);
            break;
        case 14:
            TRIL(unsigned int);
            break;
    }
    return 0;
}

