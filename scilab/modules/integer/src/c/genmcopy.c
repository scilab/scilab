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
