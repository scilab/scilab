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
