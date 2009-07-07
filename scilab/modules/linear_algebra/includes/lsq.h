/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef LSQ_H
#define LSQ_H

/*
 * Linear least square problems. cf. http://www.scilab.org/product/man/lsq.html
 * Calls Lapack [d|z]gelsy routines after performing memory allocations
 *
 * @param pMatrix double[complex]* in/out : input real or complex (in 'z' format) input matrix data, output complete orthogonal factorization
 * @param iRows int in : nb of rows of the input matrix
 * @param iCols int in : nb of cols of the input matrix
 * @param pRhs double[complex]* in : input real or complex (in 'z' format) rhs matrix data
 * @param iNRhs int in : nb of cols of the rhs matrix
 * @param complexArgs int (boolean semantics) in : whether the input matrix are real or complex
 *
 * out:
 * @param pResult double[complex]* out: iCols x nRhs solution matrix data
 *
 * in :
 * @param pTreshold double* in: treshold used to compute the effective rank of the matrix. Using pass-by-pointer because it is optional.
 * if NULL, then default value of sqrt(epsilon) is used.
 *
 * out :
 * @param piRank int* out: effective rank of input matrix. if NULL, the result is not returned.
 *
 */

extern int iLsqM(double* pMatrix, int iRows, int iCols, double* pRhs, int iNRhs, int complexArgs, double* pResult, double* pTreshold, int* piRank);


#endif
