/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
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
#include "rk4.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
/* Runge-Kutta (RK4) method */
/* http://media4.obspm.fr/public/DEA/cours/chapitre3/souschapitre2/section4/page5/section3_2_4_5.html */
/*--------------------------------------------------------------------------*/
int C2F(rk4)(double *y, double *dydx, int *n,double *x, double *h, double *yout,void (*derivs)())
{
    double d = 0.0;
    int i;
    double h6 = 0.0, hh = 0.0, xh = 0.0, yt[10], dym[10], dyt[10];

    /* Parameter adjustments (fortran)*/
    --yout;
    --dydx;
    --y;

    C2F(ierode).iero = 0;
    hh = *h * 0.5;
    h6 = *h / 6.0;
    xh = *x + hh;

    for (i = 1; i <= *n; ++i) yt[i - 1] = y[i] + hh * dydx[i];

    (*derivs)(n, &xh, yt, dyt);

    if (C2F(ierode).iero > 0) return 0;

    for (i = 1; i <= *n; ++i) yt[i - 1] = y[i] + hh * dyt[i - 1];

    (*derivs)(n, &xh, yt, dym);

    if (C2F(ierode).iero > 0) return 0;

    for (i = 1; i <= *n; ++i) 
    {
		yt[i - 1] = y[i] + *h * dym[i - 1];
		dym[i - 1] = dyt[i - 1] + dym[i - 1];
    }
    d = *x + *h;

    (*derivs)(n, &d, yt, dyt);

    if (C2F(ierode).iero > 0) return 0;

    for (i = 1; i <= *n; ++i) yout[i] = y[i] + h6 * (dydx[i] + dyt[i - 1] + dym[i - 1] * 2.0);

    return 0;
}
/*--------------------------------------------------------------------------*/
