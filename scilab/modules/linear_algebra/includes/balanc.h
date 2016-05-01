/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
#include "dynlib_linear_algebra.h"
/*
 * Implementation of Scilab balanc() function for one or two real or complex matrix (both must be of the same kind, either real or complex).
 * @param pData1 double* (or doublecomplex* disguised as double*, in fact) point to data of arg 1 matrix
 * used as output parameter for LhsVar(1) data
 * @param pData2 double* (or doublecomplex* disguised as double*, in fact) point to data of arg 2 matrix, NULL if Rhs==1
 * used as output parameter for LhsVar(2) data iff Rhs==2
 * @param iCols int nb of rows/cols of the matrix
 * @param complexArgs int (boolean semantics) if matrix are complex or real
 *
 * out:
 * @param lhsData1 double* data for LhsVar(Rhs+1)
 * @param lhsData2 double* data for LhsVar(4) if Rhs==2, NULL otherwise
 */
LINEAR_ALGEBRA_IMPEXP int iBalancM(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2);
