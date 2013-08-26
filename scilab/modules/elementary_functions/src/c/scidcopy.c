/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* rewrite scidcopy.f */
/*--------------------------------------------------------------------------*/
/* alternative to dcopy for copying array with mixed datatypes */
/* int*8 declaration used instead of double precision used to fix a */
/* efficiency problem with pentium M processors */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "scidcopy.h"
/*--------------------------------------------------------------------------*/
int C2F(scidcopy)(int *n, const long long *dx, int *incx, long long *dy, int *incy)
{
    if (*n <= 0)
    {
        return 0;
    }

    if ( (*incx == 1) && (*incy == 1) )
    {
        /*  code for both increments equal to 1 */
        /*  clean-up loop */
        memcpy(dy , dx , (*n * sizeof(double)) );
    }
    else
    {
        int i = 0;

        /* code for unequal increments or equal increments */
        /* not equal to 1 */
        int ix = *incx >= 0 ? 0 : (1 - *n) * *incx;
        int iy = *incy >= 0 ? 0 : (1 - *n) * *incy ;

        for (i = 0; i < *n; i++)
        {
            dy[iy] = dx[ix];
            ix += *incx;
            iy += *incy;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
