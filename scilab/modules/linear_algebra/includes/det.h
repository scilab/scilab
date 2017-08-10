/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
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
#ifndef DET_H
#define DET_H
#include "dynlib_linear_algebra.h"
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

LINEAR_ALGEBRA_IMPEXP int iDetM(double* pData, int iCols, double* pMantissaReal, double* pMantissaImg, int* piExponent);

#endif
