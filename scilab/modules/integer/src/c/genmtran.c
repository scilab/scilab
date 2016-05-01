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

#include "genmtran.h"

#define MTRAN(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    ia = 0;i1 = *n;\
    i2 = *m;\
    for (j = 1; j <= i1; ++j) {\
	ib = j;\
	for (i = 1; i <= i2; ++i) {\
	    B[ib] = A[ia + i];\
	    ib += *nb;\
	}\
	ia += *na;\
    }\
}


int C2F(genmtran)(int *typ, int *a, int *na, int *b, int *nb, int *m, int *n)
{
    int i1, i2;
    static int i, j, ia, ib;
    switch (*typ)
    {
        case 1:
            MTRAN(integer1);
            break;
        case 2:
            MTRAN(integer2);
            break;
        case 4:
            MTRAN(int) ;
            break;
        case 11:
            MTRAN(unsigned char);
            break;
        case 12:
            MTRAN(unsigned short);
            break;
        case 14:
            MTRAN(unsigned int);
            break;
    }
    return 0;
}

