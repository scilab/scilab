/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef DET_H
#define DET_H

/*
 * computes the determinant of a real or complex square matrix, performing MALLOCation of temp variables as needed.
 * Cf. http://www.scilab.org/product/man/det.html
 *
 * in :
 *
 * @param pData double[complex]* in/scratch data of the real or complex (in 'z' format, see below pMantissaImg) argument matrix
 * @param iCols int in nb of rows / cols of the argument matrix
 *
 * out:
 *
 * @param pMantissaReal double* ptr where the real part of the determinant [mantissa] will be stored
 * @param pMantissaImg double* ptr where the imaginary part of the determinant [mantissa] will be stored, NULL indicates that matrix is real.
 * @param piExponent int* ptr where the exponent will be stored, NULL if the determinant is not to be computed in mantissa, exponent form.
 *
 * @return error code : 0 for success, -1 for MALLOC failure, other values from LAPACK [z|g]etrf.
 *
 */

int iDetM(double* pData, int iCols, double* pMantissaReal, double* pMantissaImg, int* piExponent);

#endif
