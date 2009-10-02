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
 int getListItemNumber(int* _piAddress, int* _piNbItem);

/**
 * Get address of an item in a list
 * @param[in] _piAddress list address
 * @param[in] _iItemNum item number
 * @param[out] _piItemAddress return item address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getListItemAddress(int* _piAddress, int _iItemNum, int** _piItemAddress);

/**
 * get a list from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getTListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a list
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getMListInList(int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a list from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int getMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * create a list
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createMList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _iVar variable number
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createTList(int _iVar, int _iNbItem, int** _piAddress);

/**
 * create a list
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createNamedList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createNamedTList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _pstName variable name
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createNamedMList(char* _pstName, int _iNbItem, int** _piAddress);

/**
 * read a named list
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int readNamedList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named tlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int readNamedTList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * read a named mlist
 * @param[in] _pstName variable name
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int readNamedMList(char* _pstName, int* _piNbItem, int** _piAddress);

/**
 * create a list in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createTListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createMListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a list in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
 int createMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

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
 int getMatrixOfDoubleInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos itme position in the list
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
 int getComplexMatrixOfDoubleInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

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
 int allocMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal);

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
 int allocComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

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
 int createMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

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
 int createComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

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
 int createComplexZMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

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
 int createMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

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
 int createComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

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
 int createComplexZMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData);

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
 int readMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal);

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
 int readComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

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
 int getMatrixOfStringInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

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
 int createMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

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
 int createMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

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
 int readMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

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
 int getMatrixOfBooleanInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool);

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
 int allocMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);

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
 int createMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

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
 int createMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool);

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
 int readMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool);

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
 int getMatrixOfPolyInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

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
 int getComplexMatrixOfPolyInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
 int createMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

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
 int createComplexMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
 int readMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

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
 int readComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
 int createMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

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
 int createComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

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
 int createMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
 int createMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

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
 int createMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
 int createMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

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
 int createMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
 int createMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

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
 int createMatrixOfInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
 int createMatrixOfUnsignedInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
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
 int allocMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData);
 int allocMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData);

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
 int allocMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData);
 int allocMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData);

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
 int allocMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData);
 int allocMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData);

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
 int allocMatrixOfInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData);
 int allocMatrixOfUnsignedInteger64InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData);
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
 int getMatrixOfInteger8InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData);
 int getMatrixOfUnsignedInteger8InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData);

/**
 * read integer 16 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _psData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
 int getMatrixOfInteger16InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData);
 int getMatrixOfUnsignedInteger16InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData);

/**
 * read integer 32 variable in a list
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _iRows Number of row
 * @param[out] _iCols Number of column
 * @param[out] _piData return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
 int getMatrixOfInteger32InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData);
 int getMatrixOfUnsignedInteger32InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData);

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
 int getMatrixOfInteger64InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData);
 int getMatrixOfUnsignedInteger64InList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData);
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
 int createMatrixOfInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData);
 int createMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData);

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
 int createMatrixOfInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData);
 int createMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData);

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
 int createMatrixOfInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData);
 int createMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData);

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
 int createMatrixOfInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long* _pllData);
 int createMatrixOfUnsignedInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long* _pullData);
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
 int readMatrixOfIntger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData);
 int readMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData);

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
 int readMatrixOfIntger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData);
 int readMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData);

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
 int readMatrixOfIntger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData);
 int readMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData);

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
 int readMatrixOfIntger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long* _pllData);
 int readMatrixOfUnsignedInteger64InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long* _pullData);
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
 int createSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
 int createComplexSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

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
 int createSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
 int createComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

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
 int getSparseMatrixInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal);

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
 int getComplexSparseMatrixInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg);

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
 int readSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal);

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
 int readComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);


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
 int createBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

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
 int createBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos);

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
 int getBooleanSparseMatrixInList(int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos);

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
 int readBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos);


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
 int getPointerInList(int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _iVar variable number
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
 int createPointerInList(int _iVar, int* _piParent, int _iItemPos, void* _pvPtr);

/**
 * Get pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
 int readPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr);

/**
 * Create pointer in a list
 * @param[in] _pstName variable name
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _pvPtr return pointer value
 * @return if the operation successed (0) or not ( !0 )
 */
 int createPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr);

#ifdef __cplusplus
}
#endif
#endif /* __LIST_API__ */

