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

#ifndef __COMMON_API__
#define __COMMON_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_common.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_getVar           API_PROTO(getVar)
#define scilab_getClone         API_PROTO(getClone)
#define scilab_getType          API_PROTO(getType)
#define scilab_isDouble         API_PROTO(isDouble)
#define scilab_isBoolean        API_PROTO(isBoolean)
#define scilab_isString         API_PROTO(isString)
#define scilab_isCell           API_PROTO(isCell)
#define scilab_isInt            API_PROTO(isInt)
#define scilab_isPoly           API_PROTO(isPoly)
#define scilab_isList           API_PROTO(isList)
#define scilab_isTList          API_PROTO(isTList)
#define scilab_isMList          API_PROTO(isMList)
#define scilab_isHandle         API_PROTO(isHandle)
#define scilab_isStruct         API_PROTO(isStruct)
#define scilab_isCell           API_PROTO(isCell)
#define scilab_isPointer        API_PROTO(isPointer)
#define scilab_isComplex        API_PROTO(isComplex)
#define scilab_getDim           API_PROTO(getDim)
#define scilab_getDim2d         API_PROTO(getDim2d)
#define scilab_getDimArray      API_PROTO(getDimArray)
#define scilab_getSize          API_PROTO(getSize)
#define scilab_isEmpty          API_PROTO(isEmpty)
#define scilab_isScalar         API_PROTO(isScalar)
#define scilab_isSquare         API_PROTO(isSquare)
#define scilab_isVector         API_PROTO(isVector)
#define scilab_isMatrix2d       API_PROTO(isMatrix2d)
#define scilab_isNVector        API_PROTO(isNVector)
#define scilab_isRowVector      API_PROTO(isRowVector)
#define scilab_isColVector      API_PROTO(isColVector)
#define scilab_isHypermat       API_PROTO(isHypermat)
#define scilab_overload         API_PROTO(overload)
#define scilab_call             API_PROTO(call)

//var
/*this function should be call out of gateway so do not need scilabEnv argument*/
scilabVar API_PROTO(getVar)(const wchar_t* name);

scilabVar API_PROTO(getClone)(scilabEnv env, scilabVar var);

//type
int API_PROTO(getType)(scilabEnv env, scilabVar var);
int API_PROTO(isDouble)(scilabEnv env, scilabVar var);
int API_PROTO(isBoolean)(scilabEnv env, scilabVar var);
int API_PROTO(isString)(scilabEnv env, scilabVar var);
int API_PROTO(isCell)(scilabEnv env, scilabVar var);
int API_PROTO(isInt)(scilabEnv env, scilabVar var);
int API_PROTO(isPoly)(scilabEnv env, scilabVar var);
int API_PROTO(isList)(scilabEnv env, scilabVar var);
int API_PROTO(isTList)(scilabEnv env, scilabVar var);
int API_PROTO(isMList)(scilabEnv env, scilabVar var);
int API_PROTO(isHandle)(scilabEnv env, scilabVar var);
int API_PROTO(isStruct)(scilabEnv env, scilabVar var);
int API_PROTO(isCell)(scilabEnv env, scilabVar var);
int API_PROTO(isPointer)(scilabEnv env, scilabVar var);

//complex
int API_PROTO(isComplex)(scilabEnv env, scilabVar var);

//dimension
int API_PROTO(getDim)(scilabEnv env, scilabVar var);
int API_PROTO(getDim2d)(scilabEnv env, scilabVar var, int* row, int* col);
int API_PROTO(getDimArray)(scilabEnv env, scilabVar var, int** dims);
int API_PROTO(getSize)(scilabEnv env, scilabVar var);
int API_PROTO(isEmpty)(scilabEnv env, scilabVar var);
int API_PROTO(isScalar)(scilabEnv env, scilabVar var);
int API_PROTO(isSquare)(scilabEnv env, scilabVar var);
int API_PROTO(isMatrix2d)(scilabEnv env, scilabVar var);
int API_PROTO(isVector)(scilabEnv env, scilabVar var);
int API_PROTO(isNVector)(scilabEnv env, scilabVar var, int n);
int API_PROTO(isRowVector)(scilabEnv env, scilabVar var);
int API_PROTO(isColVector)(scilabEnv env, scilabVar var);
int API_PROTO(isHypermat)(scilabEnv env, scilabVar var);

/*functions*/
scilabStatus API_PROTO(overload)(scilabEnv env, scilabVar var, int nin, scilabVar* in, int nout, scilabVar* out);
scilabStatus API_PROTO(call)(scilabEnv env, const wchar_t* name, int nin, scilabVar* in, int nout, scilabVar* out);

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_API__ */
