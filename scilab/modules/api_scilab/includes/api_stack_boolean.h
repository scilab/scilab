/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#ifndef __STACK_BOOLEAN_API__
#define __STACK_BOOLEAN_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_boolean.h. Include api_scilab.h instead.
#endif
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get boolean variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piBool return pointer to data
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr getMatrixOfBoolean(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piBool);

/**
 * Allocate boolean variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _piBool return pointer to data
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr allocMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piBool);

/**
 * Create boolean variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piBool pointer to data
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piBool);

/**
 * Create boolean named variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piBool pointer to data
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createNamedMatrixOfBoolean(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const int* _piBool);

/**
 * Read boolean named variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piBool return pointer to data
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr readNamedMatrixOfBoolean(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piBool);

/*shortcut functions*/

/**
 * Check if the variable type is boolean
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isBooleanType(void* _pvCtx, int* _piAddress);

/**
 * Check if the variable type is boolean
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedBooleanType(void* _pvCtx, const char* _pstName);

/**
 * Get scalar boolean variable data
 * @param[in] _piAddress variable address
 * @param[out] _piBool return boolean value
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getScalarBoolean(void* _pvCtx, int* _piAddress, int* _piBool);

/**
 * Get scalar boolean variable data
 * @param[in] _pstName variable name
 * @param[out] _piBool return boolean value
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getNamedScalarBoolean(void* _pvCtx, const char* _pstName, int* _piBool);

/**
 * Create scalar boolean
 * @param[in] _iVar variable number
 * @param[in] _iBool value
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createScalarBoolean(void* _pvCtx, int _iVar, int _iBool);

/**
 * Create named scalar double
 * @param[in] _pstName variable name
 * @param[in] _iBool value
 * @return if the operation succeeded (0) or not ( !0 )
 */
int createNamedScalarBoolean(void* _pvCtx, const char* _pstName, int _iBool);

#ifdef __cplusplus
}
#endif
#endif /* __STACK_BOOLEAN_API__ */
