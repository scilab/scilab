/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * ...
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
/*--------------------------------------------------------------------------*/
#include "feval.h"
/*--------------------------------------------------------------------------*/
int C2F(parab)(int *nn, double *x1, double *x2, double *xres, int *itype)
{
    /* Computing 2nd power */
    if (*nn == 1)
    {
        xres[0] = (*x1) * (*x1);
        *itype = 0;
    }
    else
    {
        xres[0] = ((*x1) * (*x1)) + ((*x2) * (*x2));
        *itype = 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(parabc)(int *nn, double *x1, double *x2, double *xres, int *itype)
{
    /* a version which returns complex numbers */
    /* Computing 2nd power */
    if (*nn == 1)
    {
        xres[0] = (*x1) * (*x1);
        xres[1] = (*x1);
        *itype = 1;
    }
    else
    {
        xres[0] = ((*x1) * (*x1)) + ((*x2) * (*x2));
        xres[1] = *x1 + *x2;
        *itype = 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

