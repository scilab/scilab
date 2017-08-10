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

#ifndef __INTERNAL_POLY_API__
#define __INTERNAL_POLY_API__

//internal poly functions
SciErr getCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
SciErr createCommonMatrixOfPoly(void* _pvCtx, int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);
SciErr createCommonNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);
SciErr readCommonNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

#endif /* __INTERNAL_POLY_API__ */

