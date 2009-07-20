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
#ifndef LU_H
#define LU_H

/**
 * iLuM performs memory allocations and computes (calling iLu) LU decomposition 
 * in :
 * @param pData double* pointer to memory (either in 'd' or 'z' format) of argument matrix
 * @param iRows int number of rows 
 * @param iCols int number of columns 
 * @param complexArg int (really bool) tells if data is in real (in 'd' format) or complex (in 'z' format)
 * out :
 * @param pdblLData double* pointer to memory (either in 'd' or 'z' format) where L result will be stored
 * @param pdblUData double* pointer to memory (either in 'd' or 'z' format) where U result will be stored
 * @param pdblEData double* NULL if E does not need to be computed, otherwise pointer to memory
 * (either in 'd' or 'z' format) where E result will be stored
 * @return if the operation successed (true) or not (false)
 */
extern int iLuM(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pbdlEData );
extern int iLu(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pbdlEData
	       , int* piPivot, int* piWork, double* pdblWork);

#endif
