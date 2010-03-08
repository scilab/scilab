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
SciErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of wide char
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, wchar_t** _pwstStrings);

/**
 * Get strings variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pstStrings);

/**
* Create wide strings variable data
* @param[in] _iVar variable number
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[in] _pstStrings array of strings ( null terminated )
* @param[out] _piAddress return pointer on new variable
* @return if the operation successed (0) or not ( !0 )
*/

SciErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, wchar_t** _pwstStrings);

/**
 * Create strings variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createNamedMatrixOfString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char** _pstStrings);

/**
* Create wide strings variable data
* @param[in] _pstName variable name
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[in] _pstStrings array of wide strings ( null terminated )
* @param[out] _piAddress return pointer on new variable
* @return if the operation successed (0) or not ( !0 )
*/

SciErr createNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, wchar_t** _pwstStrings);

/**
 * Get strings variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piLength return strings length
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMatrixOfString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
* Get wide strings variable data
* @param[in] _pstName variable name
* @param[in] _iRows Number of row
* @param[in] _iCols Number of column
* @param[out] _piLength return strings length
* @param[in] _pstStrings array of strings ( null terminated )
* @return if the operation successed (0) or not ( !0 )
*/

SciErr readNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings);

/*shortcut functions*/

/**
 * check if the variable type is string
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isStringType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is string
 * @param[in] _pstNAme variable name
 * @return 1 for true and 0 for false
 */
int isNamedStringType(void* _pvCtx, char* _pstName);
/**
 * Get single string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedSingleString(void* _pvCtx, int* _piAddress, char** _pstData);

/**
 * Get single wide string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pwstData return allocated wide string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedSingleWideString(void* _pvCtx, int* _piAddress, wchar_t** _pwstData);

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pstData return allocated array of string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char*** _pstData);

/**
 * Get wide strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pwstData return allocated array of wide string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, wchar_t*** _pwstData);


/**
 * Get single named string variable data
 * @param[in] _pstName variable name
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedSingleString(void* _pvCtx, char* _pstName, char** _pstData);

/**
 * Get single named string variable data
 * @param[in] _pstName variable name
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedSingleWideString(void* _pvCtx, char* _pstName, wchar_t** _pwstData);

/**
 * Get named strings variable data
 * @param[in] _pstName variable name
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pstData return allocated array of string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, char*** _pstData);

/**
 * Get named wide strings variable data
 * @param[in] _pstName variable name
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pwstData return allocated array of wide string pointer ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, wchar_t*** _pwstData);

/**
 * Get single string variable data
 * @param[in] _iVar variable number
 * @param[in] _pstStrings string ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int createSingleString(void* _pvCtx, int _iVar, char* _pstStrings);

/**
 * Get single wide string variable data
 * @param[in] _iVar variable number
 * @param[in] _pwstStrings wide string ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int createSingleWideString(void* _pvCtx, int _iVar, wchar_t* _pwstStrings);

/**
 * Get named single string variable data
 * @param[in] _pstName variable name
 * @param[in] _pstStrings string ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedSingleString(void* _pvCtx, char* _pstName, char* _pstStrings);

/**
 * Get named single wide string variable data
 * @param[in] _pstName variable name
 * @param[in] _pwstStrings wide string ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedSingleWideString(void* _pvCtx, char* _pstName, wchar_t* _pwstStrings);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pstData string data
 */
void freeAllocatedSingleString(char* _pstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pwstData wide string data
 */
void freeAllocatedSingleWideString(wchar_t* _pwstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows row number
 * @param[in] _iCols column number
 * @param[in] _pstData matrix of string data
 */
void freeAllocatedMatrixOfString(int _iRows, int _iCols, char** _pstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows row number
 * @param[in] _iCols column number
 * @param[in] _pwstData matrix of wide string data
 */
void freeAllocatedMatrixOfWideString(int _iRows, int _iCols, wchar_t** _pwstData);

#ifdef __cplusplus
}
#endif

#endif /* __STRING_API__ */
