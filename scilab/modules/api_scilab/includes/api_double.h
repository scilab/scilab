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

#ifndef __DOUBLE_API__
#define __DOUBLE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_double.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createEmptyMatrix        API_PROTO(createEmptyMatrix)
#define scilab_createDoubleMatrix       API_PROTO(createDoubleMatrix)
#define scilab_createDoubleMatrix2d     API_PROTO(createDoubleMatrix2d)
#define scilab_createDouble             API_PROTO(createDouble)
#define scilab_createDoubleComplex      API_PROTO(createDoubleComplex)

#define scilab_getDoubleArray           API_PROTO(getDoubleArray)
#define scilab_getDoubleComplexArray    API_PROTO(getDoubleComplexArray)
#define scilab_getDouble                API_PROTO(getDouble)
#define scilab_setDoubleArray           API_PROTO(setDoubleArray)
#define scilab_setDoubleComplexArray    API_PROTO(setDoubleComplexArray)

/* Scilab 6 API*/
scilabVar API_PROTO(createEmptyMatrix)(scilabEnv env);
scilabVar API_PROTO(createDoubleMatrix)(scilabEnv env, int dim, const int* dims, int complex);
scilabVar API_PROTO(createDoubleMatrix2d)(scilabEnv env, int row, int col, int complex);
scilabVar API_PROTO(createDouble)(scilabEnv env, double real);
scilabVar API_PROTO(createDoubleComplex)(scilabEnv env, double real, double img);

scilabStatus API_PROTO(getDoubleArray)(scilabEnv env, scilabVar var, double** real);
scilabStatus API_PROTO(getDoubleComplexArray)(scilabEnv env, scilabVar var, double** real, double** img);
scilabStatus API_PROTO(getDouble)(scilabEnv env, scilabVar var, double* real);
scilabStatus API_PROTO(getDoubleComplex)(scilabEnv env, scilabVar var, double* real, double* img);

scilabStatus API_PROTO(setDoubleArray)(scilabEnv env, scilabVar var, const double* real);
scilabStatus API_PROTO(setDoubleComplexArray)(scilabEnv env, scilabVar var, const double* real, const double* img);
scilabStatus API_PROTO(setDouble)(scilabEnv env, scilabVar var, double real);
scilabStatus API_PROTO(setDoubleComplex)(scilabEnv env, scilabVar var, double real, double img);

#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_API__ */
