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

#ifndef __INTERNAL_DOUBLE_API__
#define __INTERNAL_DOUBLE_API__

//internal double functions
SciErr getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, char _cType, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
SciErr allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, char _cType, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
SciErr createCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);
SciErr readCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

#endif /* __INTERNAL_DOUBLE_API__ */
