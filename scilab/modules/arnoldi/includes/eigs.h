/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
 * @return <ReturnValue>
 */
int eigs(double *AR, doublecomplex *AC, int N, int Acomplex, int Asym,
         double* B,  doublecomplex* BC, int Bcomplex, int matB, int nev,
         doublecomplex* SIGMA, char* which, double* maxiter, double* tol,
         double* NCV, double* RESID, doublecomplex* RESIDC, int* INFO,
         double* cholB, int INFO_EUPD, doublecomplex* eigenvalue,
         doublecomplex* eigenvector);

#endif /* __EIGS_H__ */
/*--------------------------------------------------------------------------*/

