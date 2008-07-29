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
int C2F(readmat)  (char *name__, integer *m, integer *n, double *scimat, unsigned long name_len);


int C2F(creadmat)  (char *name__, integer *m, integer *n, double *scimat, unsigned long name_len);
int C2F(creadcmat)  (char *name__, integer *m, integer *n, double *scimat, unsigned long name_len);
int C2F(creadsmat)  (char *name__, integer *m, integer *n, double *scimat, unsigned long name_len);

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
int C2F(cwritemat)  (char *name__, integer *m, integer *n, double *mat, unsigned long name_len);
int C2F(cwritecmat)  (char *name__, integer *m, integer *n, double *mat, unsigned long name_len);
int C2F(readchain)  (char *name__, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(creadchain)  (char *name__, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(creadchains)  (char *name__, integer *ir, integer *ic, integer *itslen, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(cwritechain)  (char *name__, integer *m, char *chai, unsigned long name_len, unsigned long chai_len);
int C2F(matptr)  (char *name__, integer *m, integer *n, integer *lp, unsigned long name_len);
int C2F(cmatptr)  (char *name__, integer *m, integer *n, integer *lp, unsigned long name_len);
int C2F(cmatcptr)  (char *name__, integer *m, integer *n, integer *lp, unsigned long name_len);
int C2F(cmatsptr)  (char *name__, integer *m, integer *n, integer *ix, integer *j, integer *lp, integer *nlr, unsigned long name_len);

/** 
read a boolean matrix in scilab stack
*/
int C2F(creadbmat)(char *namex, integer *m, integer *n, int *scimat, unsigned long name_len);

/**
write a boolean matrix in scilab stack
*/
int C2F(cwritebmat)(char *namex, integer *m, integer *n, int *mat, unsigned long name_len);

/**
get pointer on a named boolean matrix 
*/
int C2F(cmatbptr)(char *namex, integer *m,integer *n,integer *lp, unsigned long name_len);

int C2F(str2name)  (char *name__, integer *id, unsigned long name_len);
int C2F(objptr)  (char *name__, integer *lp, integer *fin, unsigned long name_len);
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

int iAllocMatrixOfDouble(int _iNewVal, int _iRows, int _iCols, double **_pdblRealData);
int	iAllocComplexMatrixOfDouble(int _iNewVal, int _iComplex, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

int iAllocMatrixOfPoly(int _iNewVal, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData);
int iAllocComplexMatrixOfPoly(int _iNewVal, int _iComplex, int** _piVarName, int _iRows, int _iCols, int *_piPow, double** _pdblRealData, double** _pdblImgData);

int iAllocSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData);
int iAllocComplexSparseMatrix(int _iNewVal,int _iComplex, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow, double** _pdblRealData, double** _pdblImgData);

int iAllocMatrixOfBoolean(int _iNewVal, int _iRows, int _iCols, int** _piBoolData);
int iAllocBooleanSparseMatrix(int _iNewVal, int _iRows, int _iCols, int _iTotalElem, int** _piElemByRow, int** _piColByRow);

int iAllocMatricOfString(int _iNewVal, int _iRows, int _iCols, int *_piLen, char** _pszRealData);

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

//Internal fonctions to retrieve varaibles information from Address ( old "il" )
int iGetDoubleFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piReal, int *_piImg);
int iGetPolyFromAddress(int _iAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, int* _piReal, int *_piImg);
int iGetSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow, int* _piReal, int* _piImg);
int iGetBooleanSparseFromAddress(int _iAddr, int* _piRows, int* _piCols, int* _piTotalElem, int* _piElemByRow, int* _piColByRow);
int iGetBooleanFromAddress(int _iAddr, int *_piRows, int *_piCols, int* _piBool);
int iGetStringFromAddress(int _iAddr, int *_piRows, int *_piCols, int *_piLen, int* _piString);
#endif 
