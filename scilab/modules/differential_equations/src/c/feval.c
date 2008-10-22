/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "feval.h"
/*--------------------------------------------------------------------------*/
int C2F(parab)(int *nn, double *x1, double *x2,double *xres, int *itype, char *name, long int name_len)
{
    /* Parameter adjustments */
    --xres;

	/* Computing 2nd power */
    if (*nn == 1) 
    {
		xres[1] = (*x1) * (*x1);
		*itype = 0;
    } 
    else
    {
		xres[1] = ((*x1) * (*x1)) + ((*x2) * (*x2));
		*itype = 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(parabc)(int *nn, double *x1, double *x2,double *xres, int *itype, char *name, long int name_len)
{
	/* a version which returns complex numbers */
    /* Parameter adjustments */
    --xres;

	/* Computing 2nd power */
    if (*nn == 1) 
    {
		xres[1] = (*x1) * (*x1);
		xres[2] = (*x1);
		*itype = 1;
    } 
    else 
    {
		xres[1] = ((*x1) * (*x1)) + ((*x2) * (*x2));
		xres[2] = *x1 + *x2;
		*itype = 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/

