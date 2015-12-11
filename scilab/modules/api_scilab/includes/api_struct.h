/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enteprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    scilabStatus API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, const wchar_t* const* fields);
    scilabStatus API_PROTO(addField)(scilabEnv env, scilabVar var, const wchar_t* field);
    int API_PROTO(getFields)(scilabEnv env, scilabVar var, const wchar_t* const** fields);

    /*data*/
    scilabVar API_PROTO(getStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index);
    scilabVar API_PROTO(getStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col);

    scilabStatus API_PROTO(setStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index, const scilabVar data);
    scilabStatus API_PROTO(setStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col, const scilabVar data);

#ifdef __cplusplus
}
#endif
#endif /* __STRUCT_API__ */
