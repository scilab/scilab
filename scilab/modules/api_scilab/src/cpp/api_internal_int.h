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

#ifndef __INTERNAL_INT_API__
#define __INTERNAL_INT_API__

//internal
SciErr getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData);
SciErr allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** piData);
SciErr fillCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pdiData);
SciErr createCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int _iRows, int _iCols, const void* _piData);
SciErr readCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _piData);

static int getCommonScalarInteger(void* _pvCtx, int* _piAddress, int _iPrec, void** _pvData);
static int getCommandNamedScalarInteger(void* _pvCtx, const char* _pstName, int _iPrec, void** _pvData);
#endif /* __INTERNAL_INT_API__ */
