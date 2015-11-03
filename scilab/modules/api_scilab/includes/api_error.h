/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution. The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __ERROR_API__
#define __ERROR_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_error.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum __SCILAB_STATUS__
{
    STATUS_OK = 0,
    STATUS_ERROR = 1
}scilabStatus;

void scilab_setError(scilabEnv env, const wchar_t* msg);
const wchar_t* scilab_lastError(scilabEnv env);

#ifdef __cplusplus
}
#endif
#endif /* __ERROR_API__ */
