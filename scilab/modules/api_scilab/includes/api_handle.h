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

#ifndef __HANDLE_API__
#define __HANDLE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_handle.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createHandleMatrix       API_PROTO(createHandleMatrix)
#define scilab_createHandleMatrix2d     API_PROTO(createHandleMatrix2d)
#define scilab_createHandle             API_PROTO(createHandle)
#define scilab_getHandleArray           API_PROTO(getHandleArray)
#define scilab_setHandleArray           API_PROTO(setHandleArray)
#define scilab_getHandle                API_PROTO(getHandle)
#define scilab_setHandle                API_PROTO(setHandle)

scilabVar API_PROTO(createHandleMatrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createHandleMatrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createHandle)(scilabEnv env);

scilabStatus API_PROTO(getHandleArray)(scilabEnv env, scilabVar var, long long** vals);
scilabStatus API_PROTO(getHandle)(scilabEnv env, scilabVar var, long long* vals);
scilabStatus API_PROTO(setHandleArray)(scilabEnv env, scilabVar var, const long long* vals);
scilabStatus API_PROTO(setHandle)(scilabEnv env, scilabVar var, long long vals);
#ifdef __cplusplus
}
#endif
#endif /* __HANDLE_API__ */
