/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
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
#ifndef __EIGS_H__
#define __EIGS_H__
#include "doublecomplex.h"


/**
 * @TODO add comment
 *
 * @param AR
 * @param AC
 * @param N
 * @param Acomplex
 * @param Asym
 * @param B
 * @param BC
 * @param Bcomplex
 * @param matB
 * @param nev
 * @param SIGMA
 * @param which
 * @param maxiter
 * @param tol
 * @param NCV
 * @param RESID
 * @param RESIDC
 * @param INFO
 * @param cholB
 * @param INFO_EUPD
 * @param eigenvalue
 * @param eigenvector
 * @param eigenvalueC
 * @param eigenvectorC
 * @param RVEC
 * @return <ReturnValue>
 */
int eigs(double *AR, doublecomplex *AC, int N, int Acomplex, int Asym,
         double* B,  doublecomplex* BC, int Bcomplex, int matB, int nev,
         doublecomplex SIGMA, char* which, double* maxiter, double* tol,
         double* NCV, double* RESID, doublecomplex* RESIDC, int* INFO,
         double* cholB, int INFO_EUPD, double* eigenvalue, double* eigenvector,
         doublecomplex* eigenvalueC, doublecomplex* eigenvectorC, int RVEC);

#endif /* __EIGS_H__ */
/*--------------------------------------------------------------------------*/
