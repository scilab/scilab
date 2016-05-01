/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifndef __RK4_H__
#define __RK4_H__
/*--------------------------------------------------------------------------*/
#include "machine.h" /* C2F */
#include "dynlib_differential_equations.h"
/**
* Runge-Kutta (RK4) method
* @param y
* @param dydx
* @param n
* @param x
* @param h
* @param yout
* @param derivs
* @param rwork
* @return 0
*/
DIFFERENTIAL_EQUATIONS_IMPEXP int C2F(rk4)(double *y, double *dydx, int *n, double *x, double *h, double *yout, void (*derivs)(), double *rwork);
/*--------------------------------------------------------------------------*/
#endif /* __RK4_H__ */
/*--------------------------------------------------------------------------*/
