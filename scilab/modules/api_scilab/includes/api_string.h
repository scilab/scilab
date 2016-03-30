/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
