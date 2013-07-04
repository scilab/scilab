/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __INTERNAL_DOUBLE_API__
#define __INTERNAL_DOUBLE_API__

//internal double functions
SciErr getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, char _cType, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
SciErr allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, char _cType, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
SciErr fillCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
SciErr createCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);
SciErr readCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

#endif /* __INTERNAL_DOUBLE_API__ */
