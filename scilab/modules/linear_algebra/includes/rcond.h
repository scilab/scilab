
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2009 - INRIA
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

#ifndef __RCOND_H__
#define __RCOND_H__
#include "dynlib_linear_algebra.h"
/*
 * inverse condition number of a real or complex matrix. see http://www.scilab.org/product/man/rcond.html
 *
 * in:
 *
 * @param pData double[complex]* in/scratch real or complex (in 'z' format) argument matrix data
 * @param iCols int in nb of rows and cols of the argument matrix
 * @param complexArg int (bool semantics) in is argument matrix complex ?
 *
 * out:
 *
 * @param pRcond double* out inverse condition number
 *
 * @return error code 0: success, -1: MALLOC failure, >0 Lapack error, -> rcond= 0.
 *
 */

LINEAR_ALGEBRA_IMPEXP int iRcondM(double* pData, int iCols, int complexArg, double* pRcond);
#endif /* !__RCOND_H__ */