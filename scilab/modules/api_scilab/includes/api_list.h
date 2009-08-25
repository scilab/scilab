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

#include "dynlib_api_scilab.h"
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
API_SCILAB_IMPEXP int getListItemNumber(int* _piAddress, int* _piNbItem);

/**
 * Get address of an item in a list
 * @param[in] _piAddress list address
 * @param[in] _iItemNum item number
 * @param[out] _piItemAddress return item address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getListItemAddress(int* _piAddress, int _iItemNum, int** _piItemAddress);

/**
 * get a list from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getTListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a list from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * create a list
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createMList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createTList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a list
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedTList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedMList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * read a named list
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named tlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedTList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named mlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedMList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * create a list in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createTListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createMListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a list in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/*********************
 * Double functions *
 *********************/

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int allocMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal);

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
API_SCILAB_IMPEXP int allocComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int createMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

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
API_SCILAB_IMPEXP int createComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

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
API_SCILAB_IMPEXP int createComplexZMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

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
API_SCILAB_IMPEXP int createMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

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
API_SCILAB_IMPEXP int createComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

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
API_SCILAB_IMPEXP int createComplexZMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

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
API_SCILAB_IMPEXP int readMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal);

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
API_SCILAB_IMPEXP int readComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

/*********************
 * Strings functions *
 *********************/

/**
 * Get string variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of char
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

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
API_SCILAB_IMPEXP int createMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

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
API_SCILAB_IMPEXP int createMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

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
API_SCILAB_IMPEXP int readMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/*********************
 * boolean functions *
 *********************/

/**
 * Get boolean variable data from a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piBool return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool);

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
API_SCILAB_IMPEXP int allocMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);

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
API_SCILAB_IMPEXP int createMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

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
API_SCILAB_IMPEXP int createMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

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
API_SCILAB_IMPEXP int readMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool);

/*************************
 * polynomials functions *
 *************************/

/**
 * Get polynomial variable in list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get complex polynomial variable in list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @param[out] _pdblImg return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getComplexMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int createMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

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
API_SCILAB_IMPEXP int createComplexMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int readMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

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
API_SCILAB_IMPEXP int readComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int createMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

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
API_SCILAB_IMPEXP int createComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int createMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
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
API_SCILAB_IMPEXP int allocMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData);
API_SCILAB_IMPEXP int allocMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData);

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
API_SCILAB_IMPEXP int allocMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData);
API_SCILAB_IMPEXP int allocMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData);

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
API_SCILAB_IMPEXP int allocMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData);
API_SCILAB_IMPEXP int allocMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData);

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
API_SCILAB_IMPEXP int allocMatrixOfInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData);
API_SCILAB_IMPEXP int allocMatrixOfUnsignedInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData);
#endif
/**
 * read integer 8 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pcData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData);
API_SCILAB_IMPEXP int getMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData);

/**
 * read integer 16 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData);
API_SCILAB_IMPEXP int getMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData);

/**
 * read integer 32 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _piData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData);
API_SCILAB_IMPEXP int getMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData);

/**
 * read integer 64 variable in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _pllData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
API_SCILAB_IMPEXP int getMatrixOfInteger64InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData);
API_SCILAB_IMPEXP int getMatrixOfUnsignedInteger64InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData);
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
API_SCILAB_IMPEXP int createMatrixOfInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

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
API_SCILAB_IMPEXP int createMatrixOfInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
API_SCILAB_IMPEXP int createMatrixOfUnsignedInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
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
API_SCILAB_IMPEXP int readMatrixOfIntger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData);
API_SCILAB_IMPEXP int readMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData);

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
API_SCILAB_IMPEXP int readMatrixOfIntger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData);
API_SCILAB_IMPEXP int readMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData);

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
API_SCILAB_IMPEXP int readMatrixOfIntger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData);
API_SCILAB_IMPEXP int readMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData);

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
API_SCILAB_IMPEXP int readMatrixOfIntger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long* _pllData);
API_SCILAB_IMPEXP int readMatrixOfUnsignedInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long* _pullData);
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
API_SCILAB_IMPEXP int createSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
API_SCILAB_IMPEXP int createComplexSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

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
API_SCILAB_IMPEXP int createSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
API_SCILAB_IMPEXP int createComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

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
API_SCILAB_IMPEXP int getSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

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
API_SCILAB_IMPEXP int getComplexSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

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
API_SCILAB_IMPEXP int readSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
API_SCILAB_IMPEXP int readComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);


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
API_SCILAB_IMPEXP int createBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

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
API_SCILAB_IMPEXP int createBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

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
API_SCILAB_IMPEXP int getBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

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
API_SCILAB_IMPEXP int readBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);


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
API_SCILAB_IMPEXP int getPointerInList(int _iVar, int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createPointerInList(int _iVar, int* _piParent, int _iItemPos, void* _pvPtr);

/**
 * Get pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr);

#ifdef __cplusplus
}
#endif
#endif /* __LIST_API__ */

