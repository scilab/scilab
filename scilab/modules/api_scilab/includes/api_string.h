/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifndef __STRING_API__
#define __STRING_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_string.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createStringMatrix       API_PROTO(createStringMatrix)
#define scilab_createStringMatrix2d     API_PROTO(createStringMatrix2d)
#define scilab_createString             API_PROTO(createString)
#define scilab_getStringArray           API_PROTO(getStringArray)
#define scilab_setStringArray           API_PROTO(setStringArray)
#define scilab_getString                API_PROTO(getString)
#define scilab_setString                API_PROTO(setString)

scilabVar API_PROTO(createStringMatrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createStringMatrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createString)(scilabEnv env, const wchar_t* val);

scilabStatus API_PROTO(getStringArray)(scilabEnv env, scilabVar var, wchar_t*** strs);
scilabStatus API_PROTO(getString)(scilabEnv env, scilabVar var, wchar_t** str);
scilabStatus API_PROTO(setStringArray)(scilabEnv env, scilabVar var, const wchar_t* const* strs);
scilabStatus API_PROTO(setString)(scilabEnv env, scilabVar var, const wchar_t* str);

#ifdef __cplusplus
}
#endif
#endif /* __STRING_API__ */
