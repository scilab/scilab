/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __BOOLEAN_API__
#define __BOOLEAN_API__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get boolean variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piBool return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getMatrixOfBoolean(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piBool);

/**
 * alloc boolean variable
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piBool return pointer on data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr allocMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piBool);

/**
 * alloc boolean variable
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piBool pointer on data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piBool);

/**
 * Create boolean named variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piBool pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createNamedMatrixOfBoolean(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int* _piBool);

/**
 * Read boolean named variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piBool return data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr readNamedMatrixOfBoolean(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piBool);

#ifdef __cplusplus
}
#endif
#endif /* __BOOLEAN_API__ */
