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

#ifndef __BOOLEAN_API__
#define __BOOLEAN_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_boolean.h. Include api_scilab.h instead.
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createBooleanMatrix      API_PROTO(createBooleanMatrix)
#define scilab_createBooleanMatrix2d    API_PROTO(createBooleanMatrix2d)
#define scilab_createBoolean            API_PROTO(createBoolean)
#define scilab_getBooleanArray          API_PROTO(getBooleanArray)
#define scilab_getBoolean               API_PROTO(getBoolean)
#define scilab_setBooleanArray          API_PROTO(setBooleanArray)
#define scilab_setBoolean               API_PROTO(setBoolean)

/* Scilab 6 API*/
scilabVar API_PROTO(createBooleanMatrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createBooleanMatrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createBoolean)(scilabEnv env, int val);

scilabStatus API_PROTO(getBooleanArray)(scilabEnv env, scilabVar var, int** vals);
scilabStatus API_PROTO(getBoolean)(scilabEnv env, scilabVar var, int* val);
scilabStatus API_PROTO(setBooleanArray)(scilabEnv env, scilabVar var, const int* vals);
scilabStatus API_PROTO(setBoolean)(scilabEnv env, scilabVar var, int val);

#ifdef __cplusplus
}
#endif
#endif /* __BOOLEAN_API__ */
