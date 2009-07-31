/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
int iBalancM(double* pData1, double* pData2, int iCols, int complexArgs, double* lhsData1, double* lhsData2);
