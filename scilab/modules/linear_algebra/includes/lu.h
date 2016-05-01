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
#ifndef LU_H
#define LU_H
#include "dynlib_linear_algebra.h"
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
LINEAR_ALGEBRA_IMPEXP int iLuM(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pbdlEData );
LINEAR_ALGEBRA_IMPEXP int iLu(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pbdlEData
                              , int* piPivot, int* piWork, double* pdblWork);

#endif
