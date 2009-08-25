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

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/*******************************/
/*   boolean sparse functions   */
/*******************************/
/**
 * Get boolean sparse variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getBooleanSparseMatrix(int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Alloc boolean sparse variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int allocBooleanSparseMatrix(int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * Create boolean sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createBooleanSparseMatrix(int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

/**
 * Create named boolean sparse variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedBooleanSparseMatrix(char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

/**
 * Read named boolean sparse variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbItem return number of item
 * @param[out] _piNbItemRow return array of number of item for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedBooleanSparseMatrix(char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);

#ifdef __cplusplus
}
#endif
#endif /* __BOOLEAN_SPARSE_API__ */
