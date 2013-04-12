/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

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

