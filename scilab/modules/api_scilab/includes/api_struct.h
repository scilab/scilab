/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
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

#ifndef __STRUCT_API__
#define __STRUCT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_struct.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define scilab_createStruct             API_PROTO(createStruct)
#define scilab_createStructMatrix       API_PROTO(createStructMatrix)
#define scilab_createStructMatrix2d     API_PROTO(createStructMatrix2d)
#define scilab_addFields                API_PROTO(addFields)
#define scilab_addField                 API_PROTO(addField)
#define scilab_getFields                API_PROTO(getFields)
#define scilab_getStructMatrixData      API_PROTO(getStructMatrixData)
#define scilab_getStructMatrix2dData    API_PROTO(getStructMatrix2dData)
#define scilab_setStructMatrixData      API_PROTO(setStructMatrixData)
#define scilab_setStructMatrix2dData    API_PROTO(setStructMatrix2dData)

scilabVar API_PROTO(createStruct)(scilabEnv env);
scilabVar API_PROTO(createStructMatrix)(scilabEnv env, int dim, const int* dims);
scilabVar API_PROTO(createStructMatrix2d)(scilabEnv env, int row, int col);

/*fields*/
scilabStatus API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, const wchar_t** fields);
scilabStatus API_PROTO(addField)(scilabEnv env, scilabVar var, const wchar_t* field);
int API_PROTO(getFields)(scilabEnv env, scilabVar var, wchar_t*** fields);

/*data*/
scilabVar API_PROTO(getStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index);
scilabVar API_PROTO(getStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col);

scilabStatus API_PROTO(setStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index, scilabVar data);
scilabStatus API_PROTO(setStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col, scilabVar data);

#ifdef __cplusplus
}
#endif
#endif /* __STRUCT_API__ */
