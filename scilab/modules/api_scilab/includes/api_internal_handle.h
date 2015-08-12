/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#ifndef __INTERNAL_HANDLE_API__
#define __INTERNAL_HANDLE_API__

//internal functions
SciErr fillMatrixOfHandle(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, long long** _pllHandle);

#endif /* __INTERNAL_HANDLE_API__ */
