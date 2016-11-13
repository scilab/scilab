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

#ifndef __INTERNAL_INT_API__
#define __INTERNAL_INT_API__

//internal
SciErr getCommonMatrixOfInteger(void* _pvCtx, int* _piAddress, int _iPrecision, int* _piRows, int* _piCols, void** _piData);
SciErr allocCommonMatrixOfInteger(void* _pvCtx, int _iVar, int *_piAddress, int _iPrecision, int _iRows, int _iCols, void** piData);
SciErr createCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int _iRows, int _iCols, const void* _piData);
SciErr readCommonNamedMatrixOfInteger(void* _pvCtx, const char* _pstName, int _iPrecision, int* _piRows, int* _piCols, void* _piData);
#endif /* __INTERNAL_INT_API__ */
