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

#include "genextrac2.h"

#define EXTRAC2(Type) {\
Type *FROM;\
Type *TO;\
    FROM=(Type *)from;\
    --FROM;\
    TO=(Type *)to;\
    --TO;\
       l = 1;\
       for (j = 1; j <= *nj; ++j) \
          for (i = 1; i <= *mi; ++i) {\
	    TO[l] = FROM[indxi[i]+(indxj[j]-1)*(*m)];\
	    l = l+1;}\
}


int C2F(genextrac2)(int *typ, int *mi, int *nj, int *indxi, int *indxj, int *from, int *m, int *to)
{
    static int i, j, l;
    --indxi;
    --indxj;
    switch (*typ)
    {
        case 1:
            EXTRAC2(integer1);
            break;
        case 2:
            EXTRAC2(integer2);
            break;
        case 4:
            EXTRAC2(int) ;
            break;
        case 11:
            EXTRAC2(unsigned char);
            break;
        case 12:
            EXTRAC2(unsigned short);
            break;
        case 14:
            EXTRAC2(unsigned int);
            break;
    }
    return 0;
}

