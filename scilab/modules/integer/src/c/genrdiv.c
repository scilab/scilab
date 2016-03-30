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

#include "genrdiv.h"

#define RDIV(Type) {\
Type *A = 0;\
Type *B = 0;\
Type *R = 0;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    R=(Type *)r;\
    --R;\
    jr = 1;jb = 1;ja = 1;\
    *ierr = 0;\
    if (*ia == 0) {\
	for (k = 1; k <= i1; ++k) {\
	    if (B[jb] == 0) {*ierr = k;return 0;}\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;jb += *ib;\
	}\
    }\
    else if (*ib == 0) {\
	if (B[jb] == 0) {*ierr = 1;return 0;}\
	for (k = 1; k <= i1; ++k) {\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;ja += *ia;\
	}\
    }\
    else {\
	for (k = 1; k <= i1; ++k) {\
	    if (B[jb] == 0) {*ierr = k;return 0;}\
	    R[jr] = A[ja] / B[jb];\
	    jr += *ir;jb += *ib;ja += *ia;\
	}\
    }\
}

int C2F(genrdiv)(int *typ, int *a, int *ia, int *b, int *ib, int *r, int *ir, int *n, int *ierr)
{
    int i1 = 0;
    static int k = 0, jb = 0, ja = 0, jr = 0;

    i1 = *n;
    switch (*typ)
    {
        case 1:
            RDIV(integer1);
            break;
        case 2:
            RDIV(integer2);
            break;
        case 4:
            RDIV(int) ;
            break;
        case 11:
            RDIV(unsigned char);
            break;
        case 12:
            RDIV(unsigned short);
            break;
        case 14:
            RDIV(unsigned int);
            break;
    }
    return 0;
}

