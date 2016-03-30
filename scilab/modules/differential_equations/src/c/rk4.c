/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
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
/*--------------------------------------------------------------------------*/
#include "rk4.h"
#include "common_structure.h"
/*--------------------------------------------------------------------------*/
/* Runge-Kutta (RK4) method */
/* http://media4.obspm.fr/public/DEA/cours/chapitre3/souschapitre2/section4/page5/section3_2_4_5.html */
/*     The original version has been modified to replace statically
 *    allocated arrays yt, dym and dyt by rwork arguments parts
 *    array + blas use. Serge Steer INRIA- feb 2012*/

/*--------------------------------------------------------------------------*/
int C2F(rk4)(double *y, double *dydx, int *n, double *x, double *h, double *yout, void (*derivs)(), double *rwork)
{
    double d = 0.0;
    int i;
    double h6 = 0.0, hh = 0.0, xh = 0.0;

    double *yt = rwork;
    double *dym = rwork + *n;
    double *dyt = rwork + 2 * (*n);

    C2F(ierode).iero = 0;
    hh = *h * 0.5;
    h6 = *h / 6.0;
    xh = *x + hh;
    for (i = 0; i < *n; ++i)
    {
        yt[i] = y[i] + hh * dydx[i];
    }
    (*derivs)(n, &xh, yt, dyt);

    if (C2F(ierode).iero > 0)
    {
        return 0;
    }

    for (i = 0; i < *n; ++i)
    {
        yt[i] = y[i] + hh * dyt[i];
    }
    (*derivs)(n, &xh, yt, dym);

    if (C2F(ierode).iero > 0)
    {
        return 0;
    }

    for (i = 0; i < *n; ++i)
    {
        yt[i] = y[i] + *h * dym[i];
        dym[i] = dyt[i] + dym[i];
    }
    d = *x + *h;
    (*derivs)(n, &d, yt, dyt);

    if (C2F(ierode).iero > 0)
    {
        return 0;
    }

    for (i = 0; i < *n; ++i)
    {
        yout[i] = y[i] + h6 * (dydx[i] + dyt[i] + dym[i] * 2.0);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
