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
 * perform singular value decomposition (cf. http://www.scilab.org/product/man/svd.html ), MALLOCating temporary buffers as needed.
 *
 * in  :
 *
 * @param pData double* (doublecomplex*) in / scratch data of the argument matrix
 * @param iRows int in  nb of rows of the argument matrix
 * @param iCols int in  nb of cols of the argument matrix
 * @param complexArg int (bool semantics) in  if argument matrix is complex
 * @param economy int (bool semantics) in  if economy mode
 * @param tol double in  treshold for computing rank
 *
 * out :
 *
 * @param pSV double* out  result matrix s for lhs==1, NULL for lhs >1
 * @param pU double* out  result matrix U for lhs>1, NULL for lhs ==1
 * @param pS double* out  result matrix S for lhs>1, NULL for lhs ==1
 * @param pV double* out  result matrix V for lhs>1, NULL for lhs ==1
 * @param pRk double* out  result rank for lhs==4, NULL for lhs !=4
 *
 * @return 0 success, -1 MALLOC failure >0 Lapack convergence problems
 *
 */
int iSvdM(double* pData, int iRows, int iCols, int complexArg, int economy, double tol, double* pSV, double* pU, double* pS, double* pV, double* pRk);
