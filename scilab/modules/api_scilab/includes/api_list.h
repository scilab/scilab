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

#ifndef __LIST_API__
#define __LIST_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_list.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"

    /**********************/
    /*   list functions   */
    /**********************/

    /**
     * Get the number of items in a list
     * @param[in] _piAddress list address
     * @param[out] _piNbItem return number of items
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem);

    /**
     * Get the address of an item in a list
     * @param[in] _piAddress list address
     * @param[in] _iItemNum item number
     * @param[out] _piItemAddress return item address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress);

    /**
     * Get a list from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved list in the parent list
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get a tlist from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved tlist in the parent list
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get an mlist from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved mlist in the parent list
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get a list from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved list in the parent list
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get a tlist from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved tlist in the parent list
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getTListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Get an mlist from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the retrieved mlist in the parent list
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

    /**
     * Create a list
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create a tlist
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create an mlist
     * @param[in] _iVar variable number
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

    /**
     * Create a named list
     * @param[in] _pstName variable name
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress);

    /**
     * Create a named tlist
     * @param[in] _pstName variable name
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedTList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress);

    /**
     * Create a named mlist
     * @param[in] _pstName variable name
     * @param[in] _iNbItem number of items
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedMList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress);

    /**
     * Read a named list
     * @param[in] _pstName variable name
     * @param[out] _piNbItem return number of items
     * @param[out] _piAddress return list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress);

    /**
     * Read a named tlist
     * @param[in] _pstName variable name
     * @param[out] _piNbItem return number of items
     * @param[out] _piAddress return tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedTList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress);

    /**
     * Read a named mlist
     * @param[in] _pstName variable name
     * @param[out] _piNbItem return number of items
     * @param[out] _piAddress return mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedMList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress);

    /**
     * Create a list in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created list in the parent list
     * @param[in] _iNbItem number of items in the created list
     * @param[out] _piAddress return new list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create a tlist in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created tlist in the parent list
     * @param[in] _iNbItem number of items in the created tlist
     * @param[out] _piAddress return new tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create an mlist in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created mlist in the parent list
     * @param[in] _iNbItem number of items in the created mlist
     * @param[out] _piAddress return new mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create a list in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created list in the parent list
     * @param[in] _iNbItem number of items in the created list
     * @param[out] _piAddress return new list address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create a tlist in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created tlist in the parent list
     * @param[in] _iNbItem number of items in the created tlist
     * @param[out] _piAddress return new tlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createTListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /**
     * Create an mlist in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created mlist in the parent list
     * @param[in] _iNbItem number of items in the created mlist
     * @param[out] _piAddress return new mlist address
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

    /*********************************
     * void and undefined functions  *
     *********************************/

    /**
     * Create a void item in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created mlist in the parent list
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createVoidInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos);

    /**
     * Create a undefined item in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos position of the created mlist in the parent list
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createUndefinedInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos);

    /*********************
     * double functions  *
     *********************/

    /**
     * Get double variable data from a list item
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

    /**
     * Get complex double variable data from a list item
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

    /**
     * Allocate a double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal);

    /**
     * Allocate a complex double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

    /**
     * Create a double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal);

    /**
     * Create a complex double variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a complex double variable (Z-representation) in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblData pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData);

    /**
     * Create a double variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal);

    /**
     * Create a complex double variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a complex double variable (Z-representation) in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pdblData pointer to complex data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexZMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData);

    /**
     * Read a double variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal);

    /**
     * Read a complex double variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readComplexMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

    /*********************
     * strings functions *
     *********************/

    /**
     * Get a string variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piLength return strings length
     * @param[out] _pstStrings return array of char
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

    /**
     * Create a string variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pstStrings array of null-terminated strings
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings);

    /**
     * Create a string variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pstStrings array of null-terminated strings
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfStringInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings);

    /**
     * Read a string variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pstStrings return array of null-terminated strings
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfStringInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

    /*********************
     * boolean functions *
     *********************/

    /**
     * Get a boolean variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piBool return pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool);

    /**
     * Allocate a boolean variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piBool return pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);

    /**
     * Create a boolean variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piBool pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piBool);

    /**
     * Create a boolean variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piBool pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfBooleanInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piBool);

    /**
     * Read a boolean variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piBool pointer to boolean data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfBooleanInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool);

    /*************************
     * polynomials functions *
     *************************/

    /**
     * Get a polynomial variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomials coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

    /**
     * Get a complex polynomial variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomials coefficients
     * @param[out] _pdblImg return pointer to imaginary polynomials coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

    /**
     * Create a polynomial variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal);

    /**
     * Create a complex polynomial variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @param[in] _pdblImg pointer to imaginary polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

    /**
     * Read a polynomial variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

    /**
     * Read a complex polynomial variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbCoef return number of polynomial coefficients for each element
     * @param[out] _pdblReal return pointer to real polynomial coefficients
     * @param[out] _pdblImg return pointer to imaginary polynomial coefficients
    * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readComplexMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

    /**
     * Create a polynomial variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal);

    /**
     * Create a complex polynomial variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pstVarName polynomial variable name
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piNbCoef number of polynomial coefficients for each element
     * @param[in] _pdblReal pointer to real polynomial coefficients
     * @param[in] _pdblImg pointer to imaginary polynomial coefficients
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

    /*********************
     * integer functions *
     *********************/

    /**
     * Create an 8-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pcData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData);

    /**
     * Create an 8-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pucData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData);

    /**
     * Create a 16-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _psData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData);

    /**
     * Create a 16-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pusData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData);

    /**
     * Create a 32-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData);

    /**
     * Create a 32-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _puiData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Create a 64-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pllData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData);

    /**
     * Create a 64-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pullData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData);

#endif

    /**
     * Allocate an 8-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pcData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData);

    /**
     * Allocate an 8-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pucData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData);

    /**
     * Allocate a 16-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _psData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData);

    /**
     * Allocate a 16-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pusData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData);

    /**
     * Allocate a 32-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _piData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData);

    /**
     * Allocate a 32-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _puiData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Allocate a 64-bit signed integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData);

    /**
     * Allocate a 64-bit unsigned integer variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[out] _pullData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr allocMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData);

#endif

    /**
     * Get an 8-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pcData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData);

    /**
     * Get an 8-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pucData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData);

    /**
     * Get a 16-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _psData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData);

    /**
     * Get a 16-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pusData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData);

    /**
     * Get a 32-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData);

    /**
     * Get a 32-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _puiData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Get a 64-bit signed integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData);

    /**
     * Get a 64-bit unsigned integer variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getMatrixOfUnsignedInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData);

#endif

    /**
     * Create an 8-bit signed integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pcData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData);

    /**
     * Create an 8-bit unsigned integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pucData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData);

    /**
     * Create a 16-bit signed integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _psData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData);

    /**
     * Create a 16-bit unsigned integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pusData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData);

    /**
     * Create a 32-bit signed integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _piData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData);

    /**
     * Create a 32-bit unsigned integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _puiData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Create a 64-bit signed integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pllData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData);

    /**
     * Create a 64-bit unsigned integer variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _pullData pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData);
#endif

    /**
     * Read an 8-bit signed integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pcData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfIntger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData);

    /**
     * Read an 8-bit unsigned integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pucData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData);

    /**
     * Read a 16-bit signed integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _psData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfIntger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData);

    /**
     * Read a 16-bit unsigned integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _pusData return pointert to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData);

    /**
     * Read a 32-bit signed integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfIntger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData);

    /**
     * Read a 32-bit unsigned integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _puiData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData);

#ifdef __SCILAB_INT64__

    /**
     * Read a 64-bit signed integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows number of rows
     * @param[out] _piCols number of columns
     * @param[out] _pllData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfIntger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long* _pllData);

    /**
     * Read a 64-bit unsigned integer variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows number of rows
     * @param[out] _piCols number of columns
     * @param[out] _pullData return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long* _pullData);
#endif

    /********************
     * sparse functions *
     ********************/

    /**
     * Create a double sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal);

    /**
     * Create a complex sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

    /**
     * Create a double sparse variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal);

    /**
     * Create a complex sparse variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of item column positions ( 1 indexed )
     * @param[in] _pdblReal pointer to real parts
     * @param[in] _pdblImg pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createComplexSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

    /**
     * Get a double sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

    /**
     * Get a complex sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

    /**
     * Read a double sparse variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of item
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to real data
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

    /**
     * Read a complex sparse variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of item column positions ( 1 indexed )
     * @param[out] _pdblReal return pointer to real parts
     * @param[out] _pdblImg return pointer to imaginary parts
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readComplexSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);


    /****************************
     * boolean sparse functions *
     ****************************/

    /**
     * Create a boolean sparse variable in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

    /**
     * Create a boolean sparse variable in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _iRows number of rows
     * @param[in] _iCols number of columns
     * @param[in] _iNbItem number of items
     * @param[in] _piNbItemRow number of items for each row
     * @param[in] _piColPos array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createBooleanSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);

    /**
     * Get a boolean sparse variable from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow return number of items for each row
     * @param[out] _piColPos return array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

    /**
     * Read a boolean sparse variable from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _piRows return number of rows
     * @param[out] _piCols return number of columns
     * @param[out] _piNbItem return number of items
     * @param[out] _piNbItemRow number of items for each row
     * @param[out] _piColPos return array of column positions of "true" elements ( 1 indexed )
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readBooleanSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);

    /*********************
     * pointer functions *
     *********************/

    /**
     * Get a pointer from a list
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _pvPtr return pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr);

    /**
     * Create a pointer in a list
     * @param[in] _iVar variable number
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pvPtr return pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr);

    /**
     * Read a pointer from a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[out] _pvPtr return pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readPointerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr);

    /**
     * Create a pointer in a named list
     * @param[in] _pstName variable name
     * @param[in] _piParent pointer to the parent
     * @param[in] _iItemPos item position in the parent list
     * @param[in] _pvPtr pointer value
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createPointerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr);

    /**
     * Check if the variable type is list
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isListType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the variable type is named list
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isNamedListType(void* _pvCtx, const char* _pstName);

    /**
     * Check if the variable type is tlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isTListType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the variable type is named tlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isNamedTListType(void* _pvCtx, const char* _pstName);

    /**
     * Check if the variable type is mlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isMListType(void* _pvCtx, int* _piAddress);

    /**
     * Check if the variable type is named mlist
     * @param[in] _piAddress variable address
     * @return 1 for true and 0 for false
     */
    int isNamedMListType(void* _pvCtx, const char* _pstName);
#ifdef __cplusplus
}
#endif
#endif /* __LIST_API__ */

