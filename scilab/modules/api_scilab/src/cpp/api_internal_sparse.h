/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __INTERNAL_SPARSE_API__
#define __INTERNAL_SPARSE_API__

SciErr getCommonSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);
SciErr allocCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);
SciErr fillCommonSparseMatrix(void* _pvCtx, int *_piAddress, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double** _pdblReal, double** _pdblImg, int* _piTotalSize);
SciErr createCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);
SciErr createCommonNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);
SciErr readCommonNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);


#endif /* __INTERNAL_SPARSE_API__ */
