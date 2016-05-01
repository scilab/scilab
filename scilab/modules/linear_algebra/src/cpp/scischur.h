/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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

