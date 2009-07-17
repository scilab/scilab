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
#ifndef SAS_SCHUR_H
#define SAS_SCHUR_H

/*
 * computes Schur decomposition of matrix . Does not (yet) handle pencil or ordered variants using external functions !
 *
 * in/out data :
 *
 * @param pData1 double[complex]* in/out data of the real or complex ('z' format) arg1 matrix. output : schur
 * @param pData2 double[complex]* in/out data of the real or complex ('z' format) arg2 matrix.
 * NULL if only one argument matrix. output: generalized schur form of arg2.
 *
 * in :
 *
 * @param iCols int in : nb of rows/cols of the matrices
 * @param complexArgs int (boolean semantics) in : indicates wheter the argument[s] matrix is|are complex or real
 *
 * out data :
 *
 * @param pLhsOpt1 double[complex]* out : result of the optional lhs : [|left] schur vectors for [1|2] argument[|s].
 * NULL if optional lhs are not to be computed
 * @param pLhsOpt2 double[complex]* out : result of the optional right schur vectors (only used with 2 arguments).
 *
 * @return error code : 0 if OK, -1 if MALLOC failure, LAPACK error code otherwise.
 *
 */
int iSchurM(double* pData1, double* pData2, int iCols, int complexArgs, double* pLhsOpt1, double* pLhsOpt2);

#endif
