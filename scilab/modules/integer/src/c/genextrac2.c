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

