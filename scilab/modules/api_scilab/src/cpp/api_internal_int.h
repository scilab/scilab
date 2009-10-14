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
StrErr getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData);
StrErr allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** piData);
StrErr fillCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int _iRows, int _iCols, void** _pdiData);
StrErr createCommonNamedMatrixOfInteger(void* _pvCtx, char* _pstName, int _iPrecision, int _iRows, int _iCols, void* _piData);
StrErr readCommonNamedMatrixOfInteger(void* _pvCtx, char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _piData);

#endif /* __INTERNAL_INT_API__ */
