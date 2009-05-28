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
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getListInList(int _iVar, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getTListInList(int _iVar, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a list
 * @param[in] _iVar variable number
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getMListInList(int _iVar, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a list from a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a tlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getTListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int** _piAddress);

/**
 * get a mlist from a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent 
 * @param[in] _iItemPos position of the new list in the parent
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int getMListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int** _piAddress);

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
 * @param[in] _iNameLen variable name length
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedList(char* _pstName, int _iNameLen, int _iNbItem, int** _piAddress);

/**
 * create a tlist
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedTList(char* _pstName, int _iNameLen, int _iNbItem, int** _piAddress);

/**
 * create a mlist
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMList(char* _pstName, int _iNameLen, int _iNbItem, int** _piAddress);

/**
 * read a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedList(char* _pstName, int _iNameLen, int* _piNbItem, int** _piAddress);

/**
 * read a named tlist
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedTList(char* _pstName, int _iNameLen, int* _piNbItem, int** _piAddress);

/**
 * read a named mlist
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[out] _piNbItem return number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMList(char* _pstName, int _iNameLen, int* _piNbItem, int** _piAddress);

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
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a tlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createTListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

/**
 * create a mlist in a named list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent pointer on the parent
 * @param[in] _iItemPos position of the new list in the parent
 * @param[in] _iNbItem number of item
 * @param[out] _piAddress return list address
 * @return if the operation successed (0) or not ( !0 )
 */
int createMListInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress);

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
int getMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal);

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
int getComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

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
 * @param[in] _pdblReal return pointer on data
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
 * @param[in] _pdblReal return pointer on real data
 * @param[in] _pdblImg return pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int createComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * create double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfDoubleInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal);

/**
 * create double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal return pointer on real data
 * @param[in] _pdblImg return pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int createComplexMatrixOfDoubleInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * read double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[out] _piRows Number of row
 * @param[out] _piCols Number of column
 * @param[in] _pdblReal return data
 * @return if the operation successed (0) or not ( !0 )
 */
int readMatrixOfDoubleInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal);

/**
 * read double named variable in a list
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal return  real data
 * @param[in] _pdblImg return  imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int readComplexMatrixOfDoubleInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

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
int getMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

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
int createMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int** _piAddress);

/**
 * Write string variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfStringInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings);

/**
 * Read string variable in a named list
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _piParent address of parent node
 * @param[in] _iItemPos item postion in the list
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated )
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int readMatrixOfStringInNamedList(char* _pstName, int _iNameLen, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

#endif /* __LIST_API__ */

