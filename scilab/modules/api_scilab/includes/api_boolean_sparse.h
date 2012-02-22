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

#ifndef __BOOLEAN_SPARSE_API__
#define __BOOLEAN_SPARSE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_boolean_sparse.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************/
/*   boolean sparse functions  */
/*******************************/
/**
 * Get boolean sparse variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of number of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr getBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Allocate boolean sparse variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[out] _piNbItemRow return array of number of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr allocBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Create boolean sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of number of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

/**
 * Create named boolean sparse variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of number of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr createNamedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

/**
 * Read named boolean sparse variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
SciErr readNamedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);

/* shortcut functions */

/**
 * Check if the variable type is boolean sparse
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isBooleanSparseType(void* _pvCtx, int* _piAddress);

/**
 * Check if the variable type is boolean sparse
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedBooleanSparseType(void* _pvCtx, const char* _pstName);

/**
 * Get boolean sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of number of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getAllocatedBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Get named boolean sparse variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of number of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @return if the operation succeeded (0) or not ( !0 )
 */
int getNamedAllocatedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Free data allocated by shortcut functions
 * @param[in] _piNbItemRow array of number of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 */
void freeAllocatedBooleanSparse(int* _piNbItemRow, int* _piColPos);

#ifdef __cplusplus
}
#endif
#endif /* __BOOLEAN_SPARSE_API__ */
