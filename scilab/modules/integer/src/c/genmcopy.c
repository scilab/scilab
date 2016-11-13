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

#include "genmcopy.h"

#define MCOPY(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    if (*na == *m && *nb == *m) {\
      mn = (*m) * (*n);\
      for (i = 1; i <= mn; ++i) B[i] = A[i];\
    }\
    else {\
    ia = -(*na);ib = -(*nb); \
    for (j = 1; j <= *n; ++j) {\
      ia += *na;ib += *nb;\
      for (i = 1; i <= *m; ++i) B[ib + i] = A[ia + i];\
    }\
    }\
}
int C2F(genmcopy)(int *typ, int *a, int *na, int *b, int *nb, int *m, int *n)
{
    static int i, j, ia, ib, mn;
    switch (*typ)
    {
        case 1:
            MCOPY(integer1);
            break;
        case 2:
            MCOPY(integer2);
            break;
        case 4:
            MCOPY(int) ;
            break;
        case 11:
            MCOPY(unsigned char);
            break;
        case 12:
            MCOPY(unsigned short);
            break;
        case 14:
            MCOPY(unsigned int);
            break;
    }
    return 0;
}
