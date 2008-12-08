/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef STACK3_H
#define STACK3_H
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

/**
 * Read a matrix in scilab's internal stack
 * calling sequence
 *     logic=creadmat('matrixname',m,n,scimat)
 * @example
 *    Amat is a real 2 x 3 scilab matrix
 *    your subroutine should be as follows:
 *    subroutine mysubr(...)
 *    ...
 * @code
 *    call readmat('Amat',m,n,scimat)
 *    => m=3 , n=2, and scimat(1)=Amat(1,1)
 *                      scimat(2)=Amat(2,1)
 *                      scimat(3)=Amat(3,1)
 *                      scimat(4)=Amat(1,2) ...
 *                      scimat(5)=Amat(3,2)
 *                      scimat(6)=Amat(3,2)
 * @endcode
 * @param name__ character string; name of the scilab variable.
 * @param m number of rows
 * @param n number of columns
 * @param scimat  matrix entries stored columnwise
 * @param name_len
 * @return if the operation successed (true) or not (false)
 */
int C2F(readmat)  (char *name__, int *m, int *n, double *scimat, unsigned long name_len);


int C2F(creadmat)  (char *name__, int *m, int *n, double *scimat, unsigned long name_len);
int C2F(creadcmat)  (char *name__, int *m, int *n, double *scimat, unsigned long name_len);
int C2F(creadsmat)  (char *name__, int *m, int *n, double *scimat, unsigned long name_len);

/**
 * cwritemat writes vector/matrix in scilab's internal stack
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * @param name__ character string; name of the scilab variable ( null terMinated)
 * @param m number of rows
 * @param n number of columns
 * @param mat matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritemat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);
int C2F(cwritecmat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);
int C2F(readchain)  (char *name__, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(creadchain)  (char *name__, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(creadchains)  (char *name__, int *ir, int *ic, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(cwritechain)  (char *name__, int *m, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(matptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);
int C2F(cmatptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);
int C2F(cmatcptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);
int C2F(cmatsptr)  (char *name__, int *m, int *n, int *ix, int *j, int *lp, int *nlr, unsigned long name_len);

/**
read a boolean matrix in scilab stack
*/
int C2F(creadbmat)(char *namex, int *m, int *n, int *scimat, unsigned long name_len);

/**
write a boolean matrix in scilab stack
*/
int C2F(cwritebmat)(char *namex, int *m, int *n, int *mat, unsigned long name_len);

/**
get pointer on a named boolean matrix
*/
int C2F(cmatbptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len);

int C2F(str2name)  (char *name__, int *id, unsigned long name_len);
int C2F(objptr)  (char *name__, int *lp, int *fin, unsigned long name_len);
int C2F(putvar) (int *number, char *namex, unsigned long name_len );


/**
 * returns length of a "chain variable" in scilab
 * example :
 * in scilab --> str = "abcdefghijklmnopqrstuvwxyz";
 * in C getlengthchain("str") returns 26
 * @param namex
 * @return  -1 if error
*/
int getlengthchain(char *namex);
int iIsComplex(int _iVar);
void GetRhsPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal);
void GetRhsCPolyVar(int _iVarNum, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg);
void GetRhsSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal);
void GetRhsCSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg);
void GetRhsBooleanSparseVar(int _iVarNum, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow);

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
int iAllocComplexSparseMatrix(int _iNewVal,int _iComplex, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData, double** _pdblImgData);

int iAllocMatrixOfBoolean(int _iNewVal, int _iRows, int _iCols, int** _piBoolData);
int iAllocBooleanSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow);

int iAllocMatrixOfString(int _iNewVal, int _iRows, int _iCols, int *_piLen, char** _piStringData);
int iAllocMatrixOfStringToAddress(int _iAddr, int _iRows, int _iCols, int *_piLen, char **_piStringData);

/*Get List Information*/
//Get Item Count and type of each item
int iGetListItemType(int _iVar, int *_piItemNumber, int *_pElemType);
//Get Sparse Item
int iGetListItemSparse(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg);
//Get Poly Item
int iGetListItemPoly(int _iVar, int _iItemNumber, int **_pVarName, int *_piRows, int *_piCols, int *_piPow, double **_pdblReal, double **_pdblImg);
//Get Double Item
int iGetListItemDouble(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, double **_pdblReal, double **_pdblImg);
//Does Item Complex
int iIsComplexItemElem(int _iVar, int _iItemNumber);
//Get Item String
int iGetListItemString(int _iVar, int _iItemNumber, int *_piRows, int *_piCols, int *_piLen, char* _pszData);

/*Create List*/
//Reserved VarNum for List
int* iAllocList(int _iVar, int _iItemNumber);

//Reserved VarNum for TList
int* iAllocTList(int _iVar, int _iItemNumber);

//Reserved VarNum for MList
int* iAllocMList(int _iVar, int _iItemNumber);

//Reserved VarNum for HyperMatrix
int* iAllocHyperMatrix(int _iVar, int _iItemNumber);

//Reserved VarNum for list
int* iAllocListCommon(int _iVar, int _iItemNumber, int _iListType);

//Child
//Add Common List to ParentList ( internal use only )
int* iListAllocListCommon(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber, int _iListType);

//Add HyperMatrix to ParentList
int* iListAllocHyperMatrix(int _iVar, int* _piParentList, int _iItemPos, int _iDims);

//Add MList to ParentList
int* iListAllocMList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

//Add TList to ParentList
int* iListAllocTList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

//Add List to ParentList
int* iListAllocList(int _iVar, int* _piParentList, int _iItemPos, int _iItemNumber);

//Add real matrix in _iVar list
int iListAllocMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double **_pdblRealData);

//Add complex matrix in _iVar list
int iListAllocComplexMatrixOfDouble(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

//Add real polynomial in _iVar list
int iListAllocMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData);

//Add complex polynomial in _iVar list
int iListAllocComplexMatrixOfPoly(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData);

//Add string matrix in _iVar list
int iListAllocString(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int *_piLen, char** _pszData);

//Internal function automaticly call after the last insertion of data
void vListClose(int _iVar);
int* piGetParentNode(int* _piStart, int* _piToFind, int *_piPos);
int IsKindOfList(int* _piNode);
void vCloseNode(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);




//Internal fonctions to retrieve varaibles information from Address ( old "il" )
int iGetDoubleFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piReal, int *_piImg);
int iGetPolyFromAddress(int _iAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg);
int iGetSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg);
int iGetBooleanSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow);
int iGetBooleanFromAddress(int _iAddr, int *_piRows, int *_piCols, int* _piBool);
int iGetStringFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piLen, int* _piString);

/*Tools to convert memory matrix storage from C to Z and Z to C*/
// vGetPointerFromDoubleComplex : set the real and imaginary part of an target array from a source doublecomplex array
void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);
// oGetDoubleComplexFromPointer : returns a target doublecomplex constructed from the source real and imaginary parts.
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);

// GetRhsVarMatrixOfDouble : Returns a pointer on the data of a matrix of double.
int GetRhsVarMatrixDouble(int number, int *_iRows, int *_iCols, double **_pdblRealData);
// GetRhsVarMatrixComplex : Returns a pointer on the data of a matrix of double.
int GetRhsVarMatrixComplex(int number, int *_iRows, int *_iCols, double **_pdblRealData, double **_pdblImgData);
#endif
