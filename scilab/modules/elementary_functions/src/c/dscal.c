/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
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

/* This file is the C version of BLAS function dscal.f */
/* Used in Scilab for Mac OS X version (Fixes %nan*0 and 0*%nan) */

#include "machine.h"

void C2F(dscal)(int *n, double *da, double *dx, int *incx)
{
    int i = 0;

    if (*n < 0 || * incx < 0)
    {
        return;
    }

    for (i = 0 ; i < *n ; i += *incx)
    {
        dx[i] = dx[i] * (*da);
    }
}
