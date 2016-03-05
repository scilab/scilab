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

#include "gencheck.h"


#define CHECK(Type) {\
Type *A;\
Type *B;\
    A=(Type *)a;\
    --A;\
    B=(Type *)b;\
    --B;\
    if (*op == equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] == B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == less + great) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] != B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia; i2 += *ib;\
	}\
      }\
      else if (*op == less) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] < B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == great) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] > B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == less + equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] <= B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
      }\
      else if (*op == great + equal) {\
	for (i = 1; i <= ii1; ++i) {\
	    if (A[i1] >= B[i2]) {\
		r[i] = 1;\
	    } else {\
		r[i] = 0;\
	    }\
	    i1 += *ia;i2 += *ib;\
	}\
    }\
}




int C2F(gencheck)(int *typ, int *op, int *n, int *a, int *ia, int *b, int *ib, int *r)
{
    static int i, i1, i2, ii1;
    static int less = 59;
    static int great = 60;
    static int equal = 50;

    --r;
    i1 = 1;
    i2 = 1;
    ii1 = *n;
    switch (*typ)
    {
        case 0:
            CHECK(double);
            break;
        case 1:
            CHECK(integer1);
            break;
        case 2:
            CHECK(integer2);
            break;
        case 4:
            CHECK(int) ;
            break;
        case 11:
            CHECK(unsigned char);
            break;
        case 12:
            CHECK(unsigned short);
            break;
        case 14:
            CHECK(unsigned int);
            break;
    }
    return 0;
}
