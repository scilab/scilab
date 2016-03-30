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

#include "genconcatrow.h"

#define RCONC(Type) {\
Type *X1;\
Type *X2;\
    X1=(Type *)x1;\
    --X1;\
    X1 +=(*m1)*(*n1);\
    X2=(Type *)x2;\
    --X2;\
       for (i = 1; i <= (*m2)*(*n2); ++i) \
	 X1[i] = X2[i];}


int C2F(genconcatrow)(int *typ, int *x1, int *m1, int *n1, int *x2, int *m2, int *n2)
{
    static int i;
    switch (*typ)
    {
        case 1:
            RCONC(integer1);
            break;
        case 2:
            RCONC(integer2);
            break;
        case 4:
            RCONC(int) ;
            break;
        case 11:
            RCONC(unsigned char);
            break;
        case 12:
            RCONC(unsigned short);
            break;
        case 14:
            RCONC(unsigned int);
            break;
    }
    return 0;
}
