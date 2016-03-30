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

#include "geninsert1.h"

#define INSERT1(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       for (i = 1; i <= *mi; ++i) \
	 TO[indx[i]] = FROM[1+(i-1)*(*inc2)];\
}

int C2F(geninsert1)(int *typ, int *mi, int *indx, int *to, int *from, int *inc2)
{

    static int i;
    --indx;
    switch (*typ)
    {
        case 1:
            INSERT1(integer1);
            break;
        case 2:
            INSERT1(integer2);
            break;
        case 4:
            INSERT1(int) ;
            break;
        case 11:
            INSERT1(unsigned char);
            break;
        case 12:
            INSERT1(unsigned short);
            break;
        case 14:
            INSERT1(unsigned int);
            break;
    }
    return 0;
}

