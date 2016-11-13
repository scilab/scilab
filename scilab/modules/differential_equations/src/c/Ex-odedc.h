/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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

#ifndef __EX_ODEDC_H__
#define __EX_ODEDC_H__

void fexcd(int* jflag, int* nc, int* nd, double* t, double* y, double* ydp);
void fcd(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp);
void fc(double t, double* xc, double* u, double* xdot);
void hc(double t, double* x, double* y);
void fd(double* xd, double* y, double* xp);
void hd(double t, double* xd, double* u);
void fcd1(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp);
void fc1(double t, double* xc, double* u, double* xdot);
void hc1(double t, double* x, double* y);
void fd1(double* xd, double* y, double* xp);
void hd1(double t, double* xd, double* u);
void finput(double t, double* v);
void phis(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp);
void phit(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp);
void sbrc(double t, double* x, double* xdot);

#endif // __EX_ODEDC_H__
