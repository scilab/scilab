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

#include "gendot.h"
#include "genmmul.h"
static int c1 = 1;

#define MMUL(Type) {\
Type *A;\
Type *B;\
Type *C;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    C=(Type *)c;\
    --C;\
    for (j = 1; j <= i1; ++j) {\
	i2 = *l;\
	for (i = 1; i <= i2; ++i) \
	    C[ic + i] = C2F(gendot)(typ,m, &A[i], na, &B[ib], &c1);\
	ic += *nc;ib += *nb;\
    }\
}

int C2F(genmmul)(int *typ, int *a, int *na, int *b, int *nb, int *c, int *nc, int *l, int *m, int *n)
{
    static int i1, i2;
    static int i, j, ib, ic;
    ib = 1;
    ic = 0;
    i1 = *n;


    switch (*typ)
    {
        case 1:
            MMUL(integer1);
            break;
        case 2:
            MMUL(integer2);
            break;
        case 4:
            MMUL(int) ;
            break;
        case 11:
            MMUL(unsigned char);
            break;
        case 12:
            MMUL(unsigned short);
            break;
        case 14:
            MMUL(unsigned int);
            break;
    }
    return 0;
}

