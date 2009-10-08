/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __STRING_API__
#define __STRING_API__

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************/
/*   string matrix functions   */
/*****************************/

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of char
 * @return if the operation successed (0) or not ( !0 )
 */

StrErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of wide char
 * @return if the operation successed (0) or not ( !0 )
 */

StrErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, wchar_t** _pwstStrings);

/**
 * Get strings variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */

StrErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pstStrings);

/**
* Create wide strings variable data
* @param[in] _iVar variable number
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[in] _pstStrings array of strings ( null terminated )
* @param[out] _piAddress return pointer on new variable
* @return if the operation successed (0) or not ( !0 )
*/

StrErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, wchar_t** _pwstStrings);

/**
 * Create strings variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */

StrErr createNamedMatrixOfString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char** _pstStrings);

/**
* Create wide strings variable data
* @param[in] _pstName variable name
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[in] _pstStrings array of wide strings ( null terminated )
* @param[out] _piAddress return pointer on new variable
* @return if the operation successed (0) or not ( !0 )
*/

StrErr createNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, wchar_t** _pwstStrings);

/**
 * Get strings variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */

StrErr readNamedMatrixOfString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
* Get wide strings variable data
* @param[in] _pstName variable name
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[in] _pstStrings array of strings ( null terminated )
* @param[out] _piAddress return pointer on new variable
* @return if the operation successed (0) or not ( !0 )
*/

StrErr readNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings);

#ifdef __cplusplus
}
#endif

#endif /* __STRING_API__ */
