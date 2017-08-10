/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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

#ifndef __STACK_STRING_API__
#define __STACK_STRING_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_string.h. Include api_scilab.h instead.
#endif

#include <wchar.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/*****************************/
/*   string matrix functions   */
/*******************************/

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piLength return strings lengths
 * @param[out] _pstStrings return array of char
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Get wide strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piwLength return strings length
 * @param[out] _pwstStrings return array of wide char
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings);

/**
 * Create strings variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* const* _pstStrings);

/**
* Create wide strings variable data
* @param[in] _iVar variable number
* @param[in] _iRows Number of rows
* @param[in] _iCols Number of columns
* @param[in] _pwstStrings array of strings ( null terminated )
* @return if the operation succeeded (0) or not ( !0 )
*/
SciErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const wchar_t* const* _pwstStrings);

/**
 * Create strings variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createNamedMatrixOfString(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const char* const* _pstStrings);

/**
* Create wide strings variable data
* @param[in] _pstName variable name
* @param[in] _iRows number of rows
* @param[in] _iCols number of columns
* @param[in] _pwstStrings array of wide strings ( null terminated )
* @return if the operation succeeded (0) or not ( !0 )
*/
SciErr createNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const wchar_t* const* _pwstStrings);

/**
 * Get strings variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows number of rows
 * @param[out] _piCols number of columns
 * @param[out] _piLength return strings lengths
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr readNamedMatrixOfString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
* Get wide strings variable data
* @param[in] _pstName variable name
* @param[out] _piRows number of rows
* @param[out] _piCols number of columns
* @param[out] _piwLength return strings length
* @param[in] _pstwStrings array of strings ( null terminated )
* @return if the operation succeeded (0) or not ( !0 )
*/
SciErr readNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings);

/*shortcut functions*/

/**
 * check if the variable type is string
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isStringType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is string
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedStringType(void* _pvCtx, const char* _pstName);

/**
 * Get single string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedSingleString(void* _pvCtx, int* _piAddress, char** _pstData);

/**
 * Get single wide string variable data
 * @param[in] _piAddress variable address
 * @param[out] _pwstData return allocated wide string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedSingleWideString(void* _pvCtx, int* _piAddress, wchar_t** _pwstData);

/**
 * Get strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pstData return allocated array of string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char*** _pstData);

/**
 * Get wide strings variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pwstData return allocated array of wide string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, wchar_t*** _pwstData);


/**
 * Get single named string variable data
 * @param[in] _pstName variable name
 * @param[out] _pstData return allocated string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedNamedSingleString(void* _pvCtx, const char* _pstName, char** _pstData);

/**
 * Get single named string variable data
 * @param[in] _pstName variable name
 * @param[out] _pwstData return allocated string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedNamedSingleWideString(void* _pvCtx, const char* _pstName, wchar_t** _pwstData);

/**
 * Get named strings variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows number of rows
 * @param[out] _piCols number of columns
 * @param[out] _pstData return allocated array of string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, char*** _pstData);

/**
 * Get named wide strings variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows number of rows
 * @param[out] _piCols number of columns
 * @param[out] _pwstData return allocated array of wide string pointer ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, wchar_t*** _pwstData);

/**
 * Create single string variable data
 * @param[in] _iVar variable number
 * @param[in] _pstStrings string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createSingleString(void* _pvCtx, int _iVar, const char* _pstStrings);

/**
 * Create single string variable data
 * @param[in] _iVar variable number
 * @param[in] _iLen string length to reserve space in stack
 * @param[out] pointer on the new single string
 * @return if the operation succeeded (0) or not ( !0 )
 */
int allocSingleString(void* _pvCtx, int _iVar, int _iLen, const char** _pstStrings);

/**
 * Create single wide string variable data
 * @param[in] _iVar variable number
 * @param[in] _pwstStrings wide string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createSingleWideString(void* _pvCtx, int _iVar, const wchar_t* _pwstStrings);

/**
 * Create named single string variable data
 * @param[in] _pstName variable name
 * @param[in] _pstStrings string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createNamedSingleString(void* _pvCtx, const char* _pstName, const char* _pstStrings);

/**
 * Create named single wide string variable data
 * @param[in] _pstName variable name
 * @param[in] _pwstStrings wide string ( null terminated )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createNamedSingleWideString(void* _pvCtx, const char* _pstName, const wchar_t* _pwstStrings);

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
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pstData matrix of string data
 */
void freeAllocatedMatrixOfString(int _iRows, int _iCols, char** _pstData);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pwstData matrix of wide string data
 */
void freeAllocatedMatrixOfWideString(int _iRows, int _iCols, wchar_t** _pwstData);

#ifdef __cplusplus
}
#endif

#endif /* __STACK_STRING_API__ */
