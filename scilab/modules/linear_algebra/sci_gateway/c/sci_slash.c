
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);

extern int C2F(intdgesv4)(char *fname, unsigned long fname_len);
extern int C2F(intzgesv4)(char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
int C2F(intslash)(char *fname, unsigned long fname_len)
{
    int *header1;
    int *header2;
    int CmplxA;
    int CmplxB;
    int ret;
    int X;

    /*   X = slash(A,B) <=> X = A / B */
    header1 = (int *) GetData(1);
    header2 = (int *) GetData(2);
    CmplxA = header1[3];
    CmplxB = header2[3];
    if ((header1[2] != header2[2]) & (header1[1]*header1[2] == 1))
    {
        C2F(com).fun = 0;
        Fin = -Fin;
        return 0;
    }
    switch (CmplxA)
    {
        case REAL:
            switch (CmplxB)
            {
                case REAL :
                    /* A real, Breal */
                    ret = C2F(intdgesv4)("slash", 5L);
                    break;

                case COMPLEX :
                    /* A real, B complex : complexify A */
                    C2F(complexify)((X = 1, &X));
                    ret = C2F(intzgesv4)("slash", 5L);
                    break;

                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 2);
                    break;
            }
            return 0;

        case COMPLEX :
            switch (CmplxB)
            {
                case REAL :
                    /* A complex, B real : complexify B */
                    C2F(complexify)((X = 2, &X));
                    ret = C2F(intzgesv4)("slash", 5L);
                    break;

                case COMPLEX :
                    /* A complex, B complex */
                    ret = C2F(intzgesv4)("slash", 5L);
                    break;

                default:
                    Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                             fname, 2);
                    break;
            }
            return 0;

        default :
            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                     fname, 1);
            return 0;
    }
}
/*--------------------------------------------------------------------------*/
