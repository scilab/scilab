/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "arnol.h"
/*--------------------------------------------------------------------------*/
double aa[6] = { 1., 1., 1., 1., 1., 1. };
/*--------------------------------------------------------------------------*/
/* used in fydottable.c */
/*--------------------------------------------------------------------------*/
int C2F(arnol)(int *neq, double *t, double *y,	double *ydot)
{
    --ydot;
    --y;

    ydot[1] = aa[0] * cos(y[2]) + aa[1] * sin(y[3]);
    ydot[2] = aa[2] * cos(y[3]) + aa[3] * sin(y[1]);
    ydot[3] = aa[4] * cos(y[1]) + aa[5] * sin(y[2]);

    return 0;
}
/*--------------------------------------------------------------------------*/

