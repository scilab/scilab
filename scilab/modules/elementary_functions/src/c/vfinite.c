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


/* checks if all entries of a vector are finite */
#include "machine.h"
#include "core_math.h"
#include "vfinite.h"
#include "finite.h"


int C2F(vfinite)(int *n, double *v)
{
    int i;
    for (i = 0; i < *n; i++)
        if (finite(v[i]) == 0)
        {
            return 0;
        }
    return 1;
}

int C2F(vfiniteComplex)(int *n, doublecomplex *v)
{
    int i;
    for (i = 0; i < *n; i++)
        if (finiteComplex(v[i]) == 0)
        {
            return 0;
        }
    return 1;
}
