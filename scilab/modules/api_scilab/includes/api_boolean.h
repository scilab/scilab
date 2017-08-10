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
