/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#ifndef __EX_ODE_H__
#define __EX_ODE_H__

void fex(int* neq, double* t, double* y, double* ydot);
void jex(int* neq, double* t, double* y, double* ml, double* mu, double* pd, int* nrpd);
void fex2(int* neq, double* t, double* y, double* ydot);
void fex3(int* neq, double* t, double* y, double* ydot);
void fexab(int* neq, double* t, double* x, double* xdot);

#endif // __EX_ODE_H__
