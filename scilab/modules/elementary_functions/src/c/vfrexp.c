/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include <math.h>
#include "machine.h"
#include "vfrexp.h"


void C2F(vfrexp)(int *n, double *x, int *ix, double *y, int *iy, double *z, int *iz)
{
    int ix1, iy1, iz1, i, j;
    ix1 = 0;
    iy1 = 0;
    iz1 = 0;

    if (*ix < 0)
    {
        ix1 = -(*n - 1) * (*ix);
    }
    if (*iy < 0)
    {
        iy1 = -(*n - 1) * (*iy);
    }
    if (*iz < 0)
    {
        iz1 = -(*n - 1) * (*iz);
    }

    for (i = 0; i < *n; i++)
    {
        y[iy1] = frexp(x[ix1], &j);
        z[iz1] = j;
        iy1 += *iy;
        ix1 += *ix;
        iz1 += *iz;
    }
}
