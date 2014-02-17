/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
