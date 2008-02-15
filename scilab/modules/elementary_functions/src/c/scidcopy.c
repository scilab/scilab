/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* rewrite scidcopy.f */
/*--------------------------------------------------------------------------*/
/* alternative to dcopy for copying array with mixed datatypes */
/* integer*8 declaration used instead of double precision used to fix a */
/* efficiency problem with pentium M processors */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "scidcopy.h"
/*--------------------------------------------------------------------------*/
int C2F(scidcopy)(int *n, long long *dx, int *incx, long long *dy, int *incy)
{
    /* System generated locals */
    int i1;

    /* Local variables */
    static int i, m, ix, iy, mp1;

    /* Parameter adjustments */
    --dy;
    --dx;

    if (*n <= 0) return 0;

    if (*incx == 1 && *incy == 1) 
	{
		/*  code for both increments equal to 1 */
		/*  clean-up loop */
		m = *n % 7;
		if (m == 0)
		{
			mp1 = m + 1;
			i1 = *n;

			for (i = mp1; i <= i1; i += 7) 
			{
				dy[i] = dx[i];
				dy[i + 1] = dx[i + 1];
				dy[i + 2] = dx[i + 2];
				dy[i + 3] = dx[i + 3];
				dy[i + 4] = dx[i + 4];
				dy[i + 5] = dx[i + 5];
				dy[i + 6] = dx[i + 6];
			}
			return 0;
		}
		else
		{
			i1 = m;
			for (i = 1; i <= i1; ++i) dy[i] = dx[i];
			if (*n < 7) return 0;

			mp1 = m + 1;
			i1 = *n;

			for (i = mp1; i <= i1; i += 7) 
			{
				dy[i] = dx[i];
				dy[i + 1] = dx[i + 1];
				dy[i + 2] = dx[i + 2];
				dy[i + 3] = dx[i + 3];
				dy[i + 4] = dx[i + 4];
				dy[i + 5] = dx[i + 5];
				dy[i + 6] = dx[i + 6];
			}
			return 0;
		}
	}
	else
	{
		/* code for unequal increments or equal increments */
		/* not equal to 1 */
		ix = 1;
		iy = 1;
		if (*incx < 0) ix = (-(*n) + 1) * *incx + 1;
		if (*incy < 0) iy = (-(*n) + 1) * *incy + 1;

		i1 = *n;

		for (i = 1; i <= i1; ++i) 
		{
			dy[iy] = dx[ix];
			ix += *incx;
			iy += *incy;
		}
		return 0;
	}
	return 0;
} 
/*--------------------------------------------------------------------------*/

