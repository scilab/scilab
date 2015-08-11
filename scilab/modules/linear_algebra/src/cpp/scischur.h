/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "doublecomplex.h"

int schur_sb02mw(double* _real, double* _img); // discrete
int schur_sb02mv(double* _real, double* _img); // continu
int schur_dgees(double* _real, double* _img);

int schur_zb02mw(doublecomplex* _complex); // discrete
int schur_zb02mv(doublecomplex* _complex); // continu
int schur_zgees(doublecomplex* _complex);

int schur_sb02ox(double* _real, double* _img, double* _beta); // discrete
int schur_sb02ow(double* _real, double* _img, double* _beta); // continu
int schur_dgges(double* _real, double* _img, double* _beta);

int schur_zb02ox(doublecomplex* _alpha, doublecomplex* _beta); // discrete
int schur_zb02ow(doublecomplex* _alpha, doublecomplex* _beta); // continu
int schur_zgges(doublecomplex* _alpha, doublecomplex* _beta);

