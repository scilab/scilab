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

#ifndef __INT_API__
#define __INT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_int.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define scilab_getIntegerPrecision                  API_PROTO(getIntegerPrecision)
#define scilab_isInt8                               API_PROTO(isInt8)
#define scilab_isInt16                              API_PROTO(isInt16)
#define scilab_isInt32                              API_PROTO(isInt32)
#define scilab_isInt64                              API_PROTO(isInt64)
#define scilab_isUnsignedInt8                       API_PROTO(isUnsignedInt8)
#define scilab_isUnsignedInt16                      API_PROTO(isUnsignedInt16)
#define scilab_isUnsignedInt32                      API_PROTO(isUnsignedInt32)
#define scilab_isUnsignedInt64                      API_PROTO(isUnsignedInt64)

#define scilab_createInteger8                       API_PROTO(createInteger8)
#define scilab_createInteger16                      API_PROTO(createInteger16)
#define scilab_createInteger32                      API_PROTO(createInteger32)
#define scilab_createInteger64                      API_PROTO(createInteger64)
#define scilab_createUnsignedInteger8               API_PROTO(createUnsignedInteger8)
#define scilab_createUnsignedInteger16              API_PROTO(createUnsignedInteger16)
#define scilab_createUnsignedInteger32              API_PROTO(createUnsignedInteger32)
#define scilab_createUnsignedInteger64              API_PROTO(createUnsignedInteger64)

#define scilab_createIntegerMatrix                  API_PROTO(createIntegerMatrix)
#define scilab_createInteger8Matrix                 API_PROTO(createInteger8Matrix)
#define scilab_createInteger16Matrix                API_PROTO(createInteger16Matrix)
#define scilab_createInteger32Matrix                API_PROTO(createInteger32Matrix)
#define scilab_createInteger64Matrix                API_PROTO(createInteger64Matrix)
#define scilab_createUnsignedInteger8Matrix         API_PROTO(createUnsignedInteger8Matrix)
#define scilab_createUnsignedInteger16Matrix        API_PROTO(createUnsignedInteger16Matrix)
#define scilab_createUnsignedInteger32Matrix        API_PROTO(createUnsignedInteger32Matrix)
#define scilab_createUnsignedInteger64Matrix        API_PROTO(createUnsignedInteger64Matrix)

#define scilab_createIntegerMatrix2d                API_PROTO(createIntegerMatrix2d)
#define scilab_createInteger8Matrix2d               API_PROTO(createInteger8Matrix2d)
#define scilab_createInteger16Matrix2d              API_PROTO(createInteger16Matrix2d)
#define scilab_createInteger32Matrix2d              API_PROTO(createInteger32Matrix2d)
#define scilab_createInteger64Matrix2d              API_PROTO(createInteger64Matrix2d)
#define scilab_createUnsignedInteger8Matrix2d       API_PROTO(createUnsignedInteger8Matrix2d)
#define scilab_createUnsignedInteger16Matrix2d      API_PROTO(createUnsignedInteger16Matrix2d)
#define scilab_createUnsignedInteger32Matrix2d      API_PROTO(createUnsignedInteger32Matrix2d)
#define scilab_createUnsignedInteger64Matrix2d      API_PROTO(createUnsignedInteger64Matrix2d)

#define scilab_getInteger8                          API_PROTO(getInteger8)
#define scilab_getInteger16                         API_PROTO(getInteger16)
#define scilab_getInteger32                         API_PROTO(getInteger32)
#define scilab_getInteger64                         API_PROTO(getInteger64)
#define scilab_getUnsignedInteger8                  API_PROTO(getUnsignedInteger8)
#define scilab_getUnsignedInteger16                 API_PROTO(getUnsignedInteger16)
#define scilab_getUnsignedInteger32                 API_PROTO(getUnsignedInteger32)
#define scilab_getUnsignedInteger64                 API_PROTO(getUnsignedInteger64)

#define scilab_getIntegerArray                      API_PROTO(getIntegerArray)
#define scilab_getInteger8Array                     API_PROTO(getInteger8Array)
#define scilab_getInteger16Array                    API_PROTO(getInteger16Array)
#define scilab_getInteger32Array                    API_PROTO(getInteger32Array)
#define scilab_getInteger64Array                    API_PROTO(getInteger64Array)
#define scilab_getUnsignedInteger8Array             API_PROTO(getUnsignedInteger8Array)
#define scilab_getUnsignedInteger16Array            API_PROTO(getUnsignedInteger16Array)
#define scilab_getUnsignedInteger32Array            API_PROTO(getUnsignedInteger32Array)
#define scilab_getUnsignedInteger64Array            API_PROTO(getUnsignedInteger64Array)

#define scilab_setIntegerArray                      API_PROTO(setIntegerArray)
#define scilab_setInteger8Array                     API_PROTO(setInteger8Array)
#define scilab_setInteger16Array                    API_PROTO(setInteger16Array)
#define scilab_setInteger32Array                    API_PROTO(setInteger32Array)
#define scilab_setInteger64Array                    API_PROTO(setInteger64Array)
#define scilab_setUnsignedInteger8Array             API_PROTO(setUnsignedInteger8Array)
#define scilab_setUnsignedInteger16Array            API_PROTO(setUnsignedInteger16Array)
#define scilab_setUnsignedInteger32Array            API_PROTO(setUnsignedInteger32Array)
#define scilab_setUnsignedInteger64Array            API_PROTO(setUnsignedInteger64Array)

/* Scilab 6 API*/
int API_PROTO(getIntegerPrecision)(scilabEnv env, scilabVar var);

int API_PROTO(isInt8)(scilabEnv env, scilabVar var);
int API_PROTO(isInt16)(scilabEnv env, scilabVar var);
int API_PROTO(isInt32)(scilabEnv env, scilabVar var);
int API_PROTO(isInt64)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt8)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt16)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt32)(scilabEnv env, scilabVar var);
int API_PROTO(isUnsignedInt64)(scilabEnv env, scilabVar var);

scilabVar API_PROTO(createInteger8)(scilabEnv env, char val);
scilabVar API_PROTO(createInteger16)(scilabEnv env, short val);
scilabVar API_PROTO(createInteger32)(scilabEnv env, int val);
scilabVar API_PROTO(createInteger64)(scilabEnv env, long long val);
scilabVar API_PROTO(createUnsignedInteger8)(scilabEnv env, unsigned char val);
scilabVar API_PROTO(createUnsignedInteger16)(scilabEnv env, unsigned short val);
scilabVar API_PROTO(createUnsignedInteger32)(scilabEnv env, unsigned int val);
scilabVar API_PROTO(createUnsignedInteger64)(scilabEnv env, unsigned long long val);

scilabVar API_PROTO(createIntegerMatrix)(scilabEnv env, int prec, int dim, const int* dims);
scilabVar API_PROTO(createInteger8Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createInteger16Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createInteger32Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createInteger64Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createUnsignedInteger8Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createUnsignedInteger16Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createUnsignedInteger32Matrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createUnsignedInteger64Matrix)(scilabEnv env, int dim, const int* dims);

scilabVar API_PROTO(createIntegerMatrix2d)(scilabEnv env, int prec, int row, int col);
scilabVar API_PROTO(createInteger8Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger16Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger32Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createInteger64Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger8Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger16Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger32Matrix2d)(scilabEnv env, int row, int col);
scilabVar API_PROTO(createUnsignedInteger64Matrix2d)(scilabEnv env, int row, int col);

scilabStatus API_PROTO(getInteger8)(scilabEnv env, scilabVar var, char* val);
scilabStatus API_PROTO(getInteger16)(scilabEnv env, scilabVar var, short* val);
scilabStatus API_PROTO(getInteger32)(scilabEnv env, scilabVar var, int* val);
scilabStatus API_PROTO(getInteger64)(scilabEnv env, scilabVar var, long long* val);
scilabStatus API_PROTO(getUnsignedInteger8)(scilabEnv env, scilabVar var, unsigned char* val);
scilabStatus API_PROTO(getUnsignedInteger16)(scilabEnv env, scilabVar var, unsigned short* val);
scilabStatus API_PROTO(getUnsignedInteger32)(scilabEnv env, scilabVar var, unsigned int* val);
scilabStatus API_PROTO(getUnsignedInteger64)(scilabEnv env, scilabVar var, unsigned long long* val);

scilabStatus API_PROTO(getIntegerArray)(scilabEnv env, scilabVar var, void** vals);
scilabStatus API_PROTO(getInteger8Array)(scilabEnv env, scilabVar var, char** vals);
scilabStatus API_PROTO(getInteger16Array)(scilabEnv env, scilabVar var, short** vals);
scilabStatus API_PROTO(getInteger32Array)(scilabEnv env, scilabVar var, int** vals);
scilabStatus API_PROTO(getInteger64Array)(scilabEnv env, scilabVar var, long long** vals);
scilabStatus API_PROTO(getUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char** vals);
scilabStatus API_PROTO(getUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short** vals);
scilabStatus API_PROTO(getUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int** vals);
scilabStatus API_PROTO(getUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long** vals);

scilabStatus API_PROTO(setIntegerArray)(scilabEnv env, scilabVar var, const void* vals);
scilabStatus API_PROTO(setInteger8Array)(scilabEnv env, scilabVar var, const char* vals);
scilabStatus API_PROTO(setInteger16Array)(scilabEnv env, scilabVar var, const short* vals);
scilabStatus API_PROTO(setInteger32Array)(scilabEnv env, scilabVar var, const int* vals);
scilabStatus API_PROTO(setInteger64Array)(scilabEnv env, scilabVar var, const long long* vals);
scilabStatus API_PROTO(setUnsignedInteger8Array)(scilabEnv env, scilabVar var, const unsigned char* vals);
scilabStatus API_PROTO(setUnsignedInteger16Array)(scilabEnv env, scilabVar var, const unsigned short* vals);
scilabStatus API_PROTO(setUnsignedInteger32Array)(scilabEnv env, scilabVar var, const unsigned int* vals);
scilabStatus API_PROTO(setUnsignedInteger64Array)(scilabEnv env, scilabVar var, const unsigned long long* vals);

#ifdef __cplusplus
}
#endif
#endif /* __INT_API__ */
