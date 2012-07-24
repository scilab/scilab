/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */
/*
 * @file stack3.h
 * What is stack3 ?
 * This file contains most of the function to interface Scilab from
 * thirdparty application but almost primitive to access Scilab data
 * from the gateway
 * In theory, stack2.h & stack1.h should not be called directly
 */


#ifndef STACK3_H
#define STACK3_H

#if defined(__SCILAB_TOOLBOX__) && !defined(__INTERNAL_API_SCILAB__) && !defined(__USE_DEPRECATED_STACK_FUNCTIONS__) && !defined(__MEX_INCLUDE__)
#error Using stack3.h is deprecated. Please use api_scilab instead (try 'help api_scilab'). Note the stack3.h API will be removed after Scilab 6.0. You can define __USE_DEPRECATED_STACK_FUNCTIONS__ to bypass this error.
#endif

#include "machine.h"
#include "doublecomplex.h"

/*Constants*/
#define ROW_LETTER		'r'
#define COL_LETTER		'c'
#define STAR_LETTER		'*'
#define MTLB_LETTER		'm'

#define BY_ROWS			1
#define BY_COLS			2
#define BY_ALL			0
#define	BY_MTLB			-1

int C2F(putvar) (int *number, char *namex, unsigned long name_len );

/**
 * Returns if a variable is complex or not
 *
 * @param _iVar the matrix
 * @return 1 if is complex 0 otherwise
 */
int iIsComplex(int _iVar);
void GetRhsPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal);
void GetRhsCPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg);
void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal);
void GetRhsCSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg);
void GetRhsBooleanSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow);
void GetRhsStringVar(int _iVarNum, int* _piRows, int* _piCols, int* _piLen, char* _pstData);

void CreatePolyVarFromPtr(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double* _pdblRealData);
void CreateCPolyVarFromPtr(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double* _pdblRealData, double* _pdblImgData);
void CreateSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow, double* _pdblRealData);
void CreateCSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow, double* _pdblRealData, double* _pdblImgData);
void CreateBooleanVarFromPtr(int _iNewVal, int _iRows, int _iCols, int* _piBoolData);
void CreateCBooleanSparseVarFromPtr(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int* _piElemByRow, int* _piColByRow);



int iArraySum(int *_piArray, int _iStart, int _iEnd);
int GetDimFromVar(int _iVarNum, int _iNum/*Oo*/, int* _piVal);
void CheckVarUsed(int _iVarNum);
void CheckAllVarUsed(int _iStart, int _iEnd);
void GetVarDimension(int _iVarNum, int* _piRows, int* _piCols);
int iGetOrient(int _iVal);

/* Reserve space in stack for a matrix of double. */
int iAllocMatrixOfDouble(int _iNewVal, int _iRows, int _iCols, double **_pdblRealData);
/* Reserve space in stack for a matrix of complex. */
int iAllocComplexMatrixOfDouble(int _iNewVal, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

int iAllocComplexMatrixOfDoubleToAddress(int* _piAddr, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

int iAllocMatrixOfPoly(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData);
int iAllocComplexMatrixOfPoly(int _iNewVal, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData);
int iAllocComplexMatrixOfPolyToAddress(int _iAddr, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData);

int iAllocSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData);
int iAllocComplexSparseMatrix(int _iNewVal, int _iComplex, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData, double** _pdblImgData);

int iAllocMatrixOfBoolean(int _iNewVal, int _iRows, int _iCols, int** _piBoolData);
int iAllocBooleanSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow);

int iAllocMatrixOfString(int _iNewVal, int _iRows, int _iCols, int *_piLen, char** _piStringData);
int iAllocMatrixOfStringToAddress(int _iAddr, int _iRows, int _iCols, int *_piLen, char **_piStringData);

/*Get List Information*/
/* Get Item Count and type of each item */
int iGetListItemType(int _iVar, int* _piParentList, int *_piItemNumber, int *_pElemType);
/* Get SubList reference */
int* iGetListItemList(int _iVar, int* _piParentList, int iItemPos);
/* Get Sparse Item */
int iGetListItemSparse(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg);
/* Get Poly Item */
int iGetListItemPoly(int _iVar, int _iItemNumber, int **_pVarName, int *_piRows, int *_piCols, int *_piPow, double **_pdblReal, double **_pdblImg);
/* Get Double Item */
int iGetListItemDouble(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, double **_pdblReal, double **_pdblImg);
/* Does Item Complex */
int iIsComplexItemElem(int _iVar, int _iItemNumber);
/* Get Item String */
int iGetListItemString(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int *_piLen, char* _pszData);

/* Get SubItem String */
int iGetListSubItemString(int _iVar, int* _piParentList, int _iItemNumber, int *_piRows, int *_piCols, int *_piLen, char* _pszData);

/*Create List*/
/* Reserved VarNum for List */
int* iAllocList(int _iVar, int _iItemNumber);

/* Reserved VarNum for TList */
int* iAllocTList(int _iVar, int _iItemNumber);

/* Reserved VarNum for MList */
int* iAllocMList(int _iVar, int _iItemNumber);

/* Reserved VarNum for HyperMatrix */
int* iAllocHyperMatrix(int _iVar, int _iItemNumber);

/* Reserved VarNum for list */
int* iAllocListCommon(int _iVar, int _iItemNumber, int _iListType);

/* Child */
/* Add Common List to ParentList ( internal use only ) */
int* iListAllocListCommon(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber, int _iListType);

/* Add HyperMatrix to ParentList */
int* iListAllocHyperMatrix(int _iVar, int* _piParentList, int _iItemPos, int _iDims);

/* Add MList to ParentList */
int* iListAllocMList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

/* Add TList to ParentList */
int* iListAllocTList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

/* Add List to ParentList */
int* iListAllocList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

/* Add real matrix in _iVar list */
int iListAllocMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double **_pdblRealData);

/* Add complex matrix in _iVar list */
int iListAllocComplexMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

/* Add real polynomial in _iVar list */
int iListAllocMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData);

/* Add complex polynomial in _iVar list */
int iListAllocComplexMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData);

/* Add string matrix in _iVar list */
int iListAllocString(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int *_piLen, char** _pszData);

/* Internal function automaticly call after the last insertion of data */
void vListClose(int _iVar);
int* piGetParentNode(int* _piStart, int* _piToFind, int *_piPos);
int IsKindOfList(int* _piNode);
void vCloseNode(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);


/* Functions to retrieve variables information from pointer ( real memory address ) */
int iGetStringFromPointer(int* _piAddr, int *_piRows, int *_piCols, int *_piLen, int** _piString);

int* iGetAddressFromItemPos(int *_piParent, int _iItemPos);


/* Internal functions to retrieve varaibles information from Address ( old "il" ) */
int iGetDoubleFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piReal, int *_piImg);
int iGetPolyFromAddress(int _iAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg);
int iGetSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg);
int iGetBooleanSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow);
int iGetBooleanFromAddress(int _iAddr, int *_piRows, int *_piCols, int* _piBool);
int iGetStringFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piLen, int* _piString);

/**
*  Set the target real and imaginary part of an array from a source doublecomplex array.
*  @param _poComplex the source array
*  @param _iSize the number of elements to set
*  @param _pdblReal, _pdblImg the target array (real and imaginary parts)
*/
void vGetPointerFromDoubleComplex(const doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);

/**
*  Returns a target doublecomplex array constructed from the source real and imaginary parts.
*  The real and imaginary parts can be NULL or not NULL :
*  * if real part and imaginary part of source array are not NULL, the returned array is as expected,
*  * if real part of source array is NULL and imaginary part is not NULL, the real part of the returned array is filled with zeros,
*  * if real part of source array is not NULL and imaginary part is NULL, the imaginary part of the returned array is filled with zeros,
*  * if both real and imaginary parts of source array are NULL, the returned array is NULL.
*  @param _pdblReal the real part of the source array
*  @param _pdblImg the imaginary part of the source array
*  @param _iSize the size of the source array
*  @return a target doublecomplex array constructed from the source real and imaginary parts.
*/
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);
/**
*  Free the given pointer of double complex.
*  Note
*  The goal of this function is to allow the client
*  code to be independent of the particular allocation system used
*  in oGetDoubleComplexFromPointer.
*  @param _poComplex the array to free
*/
void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex);

/**
* Returns a pointer on the data of a matrix of double
*  @param number index of the Scilab variable
*  @param _iRows number of rows in the matrix
*  @param _iCols number of columns in the matrix
*  @param _pdblRealData pointer to the block of data for real values
*  @return a pointer on the data of a matrix of double
*/
int GetRhsVarMatrixDouble(int number, int *_iRows, int *_iCols, double **_pdblRealData);

/**
*  Returns a pointer on the data of a matrix of double.
*  @param number index of the Scilab variable
*  @param _iRows number of rows in the matrix
*  @param _iCols number of columns in the matrix
*  @param _pdblRealData pointer to the block of data for real values
*  @param _pdblImgData pointer to the block of data for complex values
*  @return a pointer on the data of a matrix of double.
*/
int GetRhsVarMatrixComplex(int number, int *_iRows, int *_iCols, double **_pdblRealData, double **_pdblImgData);

/* functions ONLY for compatibility and/or internal use */
int C2F(str2name)  (const char *name__, int *id, unsigned long name_len);
int C2F(objptr)(char *namex, int *lp, int *fin, unsigned long name_len);
int C2F(creadmat)(char *namex, int *m, int *n, double *scimat, unsigned long name_len);
int C2F(creadchain)(char *namex,  int *itslen,  char *chai,  unsigned long name_len,  unsigned long chai_len);
int C2F(cmatptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len);


#endif
