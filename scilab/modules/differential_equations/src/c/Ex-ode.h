/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
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

#ifndef __EX_ODE_H__
#define __EX_ODE_H__

void fex(int* neq, double* t, double* y, double* ydot);
void jex(int* neq, double* t, double* y, double* ml, double* mu, double* pd, int* nrpd);
void fex2(int* neq, double* t, double* y, double* ydot);
void fex3(int* neq, double* t, double* y, double* ydot);
void fexab(int* neq, double* t, double* x, double* xdot);

#endif // __EX_ODE_H__
