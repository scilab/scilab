/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - Université de Nancy - Bruno Pincon <Bruno.Pincon@iecn.u-nancy.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "stack-c.h"
#include "gw_special_functions2.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double F2C(dgammacody)(double *);
extern double F2C(betaln)(double *, double *);
/*--------------------------------------------------------------------------*/
int sci_beta(char *fname,unsigned long fname_len)
{
  /*
   *   z = beta(x, y)
   *
   *      x, y : matrices of the same size of positive reals
   *
   *   For small x+y values uses the expression with the
   *   gamma function, else the exponential applied on the log beta
   *   function (provided in the dcd lib)
   *
   *   The switch limit have been set by using the gp-pari software.
   *
   */
	int mx = 0, nx = 0, itx = 0, lx = 0, lxc = 0, my = 0, ny = 0, ity = 0, ly = 0, lyc = 0, lz = 0, i = 0;
	double *x = NULL, *y = NULL, *z = NULL, xpy = 0.;
	double switch_limit = 2;

	CheckLhs(1,1);
	CheckRhs(2,2);

	GetRhsCVar(1,MATRIX_OF_DOUBLE_DATATYPE, &itx, &mx, &nx, &lx, &lxc);
	x = stk(lx);

	GetRhsCVar(2,MATRIX_OF_DOUBLE_DATATYPE, &ity, &my, &ny, &ly, &lyc);
	y = stk(ly);

	CheckSameDims(1,2,mx,nx,my,ny);

	if ( (itx == 1)  ||  (ity == 1) )
	{
		Scierror(999,_("%s: Wrong type for input argument #%d or #%d: No complex input argument expected.\n"), fname, 1, 2);
		return 0;
	};

	for ( i = 0 ; i < mx*nx ; i++ )
	{
		if ( (x[i] <= 0.0)  ||  (y[i] <= 0.0) )
		{
			Scierror(999,_("%s: Wrong value for input arguments: Must be > %d.\n"), fname,0);
			return 0;
		}
	}

	CreateVar(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lz); 
	z = stk(lz);

	for ( i = 0 ; i < mx*nx ; i++ )
	{
		xpy = x[i] + y[i];

		if ( xpy <= switch_limit )
		{
			z[i] = F2C(dgammacody)(&x[i]) * F2C(dgammacody)(&y[i]) / F2C(dgammacody)(&xpy);
		}
		else
		{
			z[i] = exp(F2C(betaln)(&x[i], &y[i]));
		}
    }

	LhsVar(1) = Rhs + 1;
	return 0;
}
/*--------------------------------------------------------------------------*/
