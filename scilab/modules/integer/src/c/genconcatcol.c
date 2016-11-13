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

#include "genconcatcol.h"


#define COLCONC(Type) {\
Type *X1;\
Type *X2;\
Type *XR;\
    X1=(Type *)x1;\
    --X1;\
    X2=(Type *)x2;\
    --X2;\
    XR=(Type *)xr;\
    --XR;\
    i1 = -(*m1);i2 = -(*m2);\
    ir1 = -m; ir2 = ir1 + (*m1);\
    for (j = 1; j <= *n1; ++j) {\
      i1 += *m1; i2 += *m2; \
      ir1 += m; ir2 += m; \
      for (i = 1; i <= *m1; ++i) XR[ir1 + i] = X1[i1 + i]; \
      for (i = 1; i <= *m2; ++i) XR[ir2 + i] = X2[i2 + i]; \
     }\
}


int C2F(genconcatcol)(int *typ, int *x1, int *m1, int *n1, int *x2, int *m2, int *n2, int *xr)
{
    static int i, j, i1, i2, ir1, ir2;
    int m;

    m = (*m1) + (*m2);
    switch (*typ)
    {
        case 1:
            COLCONC(integer1);
            break;
        case 2:
            COLCONC(integer2);
            break;
        case 4:
            COLCONC(int) ;
            break;
        case 11:
            COLCONC(unsigned char);
            break;
        case 12:
            COLCONC(unsigned short);
            break;
        case 14:
            COLCONC(unsigned int);
            break;
    }
    return 0;
}
