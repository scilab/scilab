/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

