
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
extern int C2F(intdgetrf)(char *fname, unsigned long fname_len);
extern int C2F(intzgetrf)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(intlu)(char *fname, unsigned long fname_len)
{
    int *header1;
    int CmplxA;
    int ret;

    /*   lu(A)  */
    if (GetType(1) != sci_matrix)
    {
        OverLoad(1);
        return 0;
    }
    header1 = (int *) GetData(1);
    CmplxA = header1[3];
    switch (CmplxA)
    {
        case REAL:
            ret = C2F(intdgetrf)("lu", 2L);
            break;

        case COMPLEX:
            ret = C2F(intzgetrf)("lu", 2L);
            break;

        default:
            Scierror(999, _("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
                     fname, 1);
            break;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
