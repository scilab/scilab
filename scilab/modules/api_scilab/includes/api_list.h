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

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"

/**********************/
/*   list functions   */
/**********************/

/**
 * Get number of item in a list
 * @param[in] _piAddress list address
 * @param[out] _piNbItem return number of item
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem);

/**
 * Get address of an item in a list
 * @param[in] _piAddress list address
 * @param[in] _iItemNum item number
 * @param[out] _piItemAddress return item address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress);

/**
 * get a list from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a list from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * create a list
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a list
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedTList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createNamedMList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress);

/**
 * read a named list
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named tlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedTList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named mlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readNamedMList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * create a list in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a list in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/*********************
 * Double functions *
 *********************/

/**
 * Get double variable data
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos itme position in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos itme position in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Alloc double variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal);

/**
 * Alloc double variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

/**
 * create double variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

/**
 * create double variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * create double variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblData pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * create double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

/**
 * create double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * create double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblData pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexZMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * read double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _pdblReal return data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal);

/**
 * read double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal return  real data
 * @param[in] _pdblImg return  imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

/*********************
 * Strings functions *
 *********************/

/**
 * Get string variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of char
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Create string variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

/**
 * Write string variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

/**
 * Read string variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/*********************
 * boolean functions *
 *********************/

/**
 * Get boolean variable data from a list
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piBool return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool);

/**
 * Alloc boolean variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piBool return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);

/**
 * create boolean variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piBool  pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

/**
 * create boolean variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piBool pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

/**
 * Read string variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piBool pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool);

/*************************
 * polynomials functions *
 *************************/

/**
 * Get polynomial variable in list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get complex polynomial variable in list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @param[out] _pdblImg return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Create polynomial variable in list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * Create complex polynomial variable in list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get polynomial variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return polynomials coefficients
 * @param[out] _pdblReal return data
 * @return if the operation successed (0) or not ( !0 )
 */
SciErr readMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get polynomial variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return polynomials coefficients
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return img data
* @return if the operation successed (0) or not ( !0 )
 */

SciErr readComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Create polynomial variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * Create complex polynomial variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/*********************
 * integer functions *
 *********************/

/**
 * create integer 8 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pcData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
SciErr createMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

/**
 * create integer 16 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
SciErr createMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

/**
 * create integer 32 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
SciErr createMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

/**
 * create integer 64 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pllData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
SciErr createMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
#endif

/**
 * alloc integer 8 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData);
SciErr allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData);

/**
 * alloc integer 16 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData);
SciErr allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData);

/**
 * alloc integer 32 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData);
SciErr allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData);

/**
 * alloc integer 32 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr allocMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData);
SciErr allocMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData);
#endif
/**
 * read integer 8 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pcData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData);
SciErr getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData);

/**
 * read integer 16 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData);
SciErr getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData);

/**
 * read integer 32 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _piData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData);
SciErr getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData);

/**
 * read integer 64 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pllData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr getMatrixOfInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData);
SciErr getMatrixOfUnsignedInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData);
#endif

/**
 * Create integer 8 bis variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pcData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
SciErr createMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

/**
 * Create integer 16 bis variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _psData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
SciErr createMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

/**
 * Create integer 32 bis variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createMatrixOfInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
SciErr createMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

/**
 * Create integer 64 bis variable in named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pllData pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
SciErr createMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
#endif

/**
 * read integer 8 named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _pcData return data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readMatrixOfIntger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData);
SciErr readMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData);

/**
 * read integer 16 named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _psData return data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readMatrixOfIntger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData);
SciErr readMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData);

/**
 * read integer 32 named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _piData return data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readMatrixOfIntger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData);
SciErr readMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData);

/**
 * read integer 64 named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _pllData return data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
SciErr readMatrixOfIntger64InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long* _pllData);
SciErr readMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long* _pullData);
#endif

/********************
 * sparse functions *
 ********************/

/**
 * Create complex sparse variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Create complex sparse variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/**
 * Create sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * Create complex sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

/**
 * get complex sparse variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[out] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[out] _piNbItemRow array of number of item for each row
 * @param[out] _piColPos array of item column position ( 1 indexed )
 * @param[out] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

/**
 * get complex sparse variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[out] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[out] _piNbItemRow array of number of item for each row
 * @param[out] _piColPos array of item column position ( 1 indexed )
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

/**
 * read sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

/**
 * read complex sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);


/****************************
 * boolean sparse functions *
 ****************************/

/**
 * Create boolean variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

/**
 * Create boolean sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _iNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

/**
 * get boolean sparse variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[out] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[out] _piNbItemRow array of number of item for each row
 * @param[out] _piColPos array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

/**
 * read sparse variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _piItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[out] _piNbItem Number of item
 * @param[in] _piNbItemRow array of number of item for each row
 * @param[in] _piColPos array of item column position ( 1 indexed )
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);

/*********************
 * pointer functions *
 *********************/

/**
 * Get pointer in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr);

/**
 * Get pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr readPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */

SciErr createPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr);

#ifdef __cplusplus
}
#endif
#endif /* __LIST_API__ */

