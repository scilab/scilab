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

#ifndef QR_H
#define QR_H

/*
  Performs memory allocations and computes QR decomposition (calling iQr)
 * in :
 * @param pData double* pointer to memory (either in 'd' or 'z' format) of argument matrix
 * @param iRows int number of rows 
 * @param iCols int number of columns 
 * @param complexArg int (really bool) tells if data is in real (in 'd' format) or complex (in 'z' format)
 * @param iRowsToCompute int nb of rows to compute, iRows , or min(iRows, icols) for "economy" mode
 * @param dblTol double tolerance used for rank if -1, R(1,1)*%eps*max(iRows, iCols)
 * out :
 * @param pdblQ double* pointer to memory (either in 'd' or 'z' format) where Q result will be stored
 * @param pdblR double* pointer to memory (either in 'd' or 'z' format) where R result will be stored
 * @param pdblEData double* NULL if E does not need to be computed, otherwise pointer to memory
 * (either in 'd' or 'z' format) where E result will be stored
 * @param pdblRank double*  NULL if rank does not need to be computed, otherwise pointer to memory where rank will be stored
 * @return if the operation successed (true) or not (false)
 */  
int iQrM(double* pData, int iRows, int iCols, int /*bool*/ complexArg, int iRowsToCompute, double dblTol
	 , double* pdblQ, double* pdblR, double* pdblE, double* pdblRank);
#endif
