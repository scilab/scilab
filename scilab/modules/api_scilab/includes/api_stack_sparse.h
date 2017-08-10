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

#ifndef __STACK_SPARSE_API__
#define __STACK_SPARSE_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_sparse.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/*****************************/
/*   sparse matrix functions */
/*****************************/

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get complex sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real parts
 * @param[out] _pdblImg return pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * Allocate a sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer to data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Allocate a complex sparse variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real parts
 * @param[out] _pdblImg return pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * Create a sparse double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows nmber of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of numbers of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @param[in] _pdblReal pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal);

/**
 * Create a complex sparse double variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of numbers of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @param[in] _pdblReal pointer to real parts
 * @param[in] _pdblImg pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

/**
 * Create a named sparse double variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of numbers of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @param[in] _pdblReal pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal);

/**
 * Create a named complex sparse double variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _iNbItem number of items
 * @param[in] _piNbItemRow array of number of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @param[in] _pdblReal pointer to real parts
 * @param[int] _pdblImg pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

/**
 * Read named sparse double variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Read a named complex sparse double variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of number of items for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal pointer to real parts
 * @param[out] _pdblImg pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/* shortcut functions */

/**
 * Check if the variable type is sparse double
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isSparseType(void* _pvCtx, int* _piAddress);

/**
 * Check if the variable type is sparse double
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedSparseType(void* _pvCtx, const char* _pstName);

/**
 * Get sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get complex sparse variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real parts
 * @param[out] _pdblImg return pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * Get named sparse variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column positions ( 1 indexed )
 * @param[out] _pdblReal return pointer to real data
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedAllocatedSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * Get named complex sparse variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbItem return number of items
 * @param[out] _piNbItemRow return array of numbers of items for each row
 * @param[out] _piColPos return array of item column position ( 1 indexed )
 * @param[out] _pdblReal return pointer to real parts
 * @param[out] _pdblImg return pointer to imaginary parts
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedAllocatedComplexSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);


/**
 * Free memory allocated for a sparse matrix (named or not) by shortcut functions
 * @param[in] _piNbItemRow array of numbers of items for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer to real data
 */
void freeAllocatedSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal);

/**
 * Free memory allocated for a complex sparse matrix (named or not) by shortcut functions
 * @param[in] _piNbItemRow array of numbers of items for each row
 * @param[in] _piColPos array of item column positions ( 1 indexed )
 * @param[in] _pdblReal pointer to real parts
 * @param[in] _pdblImg pointer to imaginary parts
 */
void freeAllocatedComplexSparseMatrix(int* _piNbItemRows, int* _piColPos, double* _pdblReal, double* _pdblImg);

#ifdef __cplusplus
}
#endif
#endif /* __STACK_SPARSE_API__ */
