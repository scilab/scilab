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

#include "gentriu.h"


#define TRIU(Type) {\
Type *DX;\
    DX=(Type *)dx;\
    kk = *k;\
    nn = *n;\
    l = 0;\
    if (kk>0) {\
      for (i = 0; i < (*m)*kk; ++i) \
         DX[i] = 0;\
      l = l+ (*m)*kk;\
      nn = nn - kk;\
      kk = 0;}\
    ls = l +1 - kk;\
    ll = *m -1 + kk;\
    for (j = 1; j <= nn; ++j) {\
      if (ll > 0) {\
        for (i = 0; i < ll; ++i) \
          DX[ls+i] = 0;\
        ll--;\
        ls = ls + *m +1;\
      }\
    }\
 }

int C2F(gentriu)(int *typ, int *m, int *n, int *k, int *dx)
{
    static int kk, l, ls, ll, j, i, nn;

    switch (*typ)
    {
        case 1:
            TRIU(integer1);
            break;
        case 2:
            TRIU(integer2);
            break;
        case 4:
            TRIU(int) ;
            break;
        case 11:
            TRIU(unsigned char);
            break;
        case 12:
            TRIU(unsigned short);
            break;
        case 14:
            TRIU(unsigned int);
            break;
    }
    return 0;
}

