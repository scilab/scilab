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
 * Read a matrix in scilab's internal memory 
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
 * cwritemat writes vector/matrix in scilab's memory
 * logic=cwritemat('matrixname'//char(0),m,n,mat)
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritemat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);


/**
 * cwritemat writes vector/matrix of complex in scilab's memory
 * logic=cwritecmat('matrixname'//char(0),m,n,mat)
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat complex matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritecmat)  (char *name__, int *m, int *n, double *mat, unsigned long name_len);
int C2F(readchain)  (char *name__, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);


/**
 * Read a string in scilab's memory
 * calling sequence 
 *     logic=creadchain('matrixname',size,string)
 * @param name__ character string; name of the scilab variable. 
 * @param itslen lenght of the string
 * @param chai the future string
 * @param name_len strlen of name__ (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadchain)  (char *name__, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);


/**
 * Read a string from a matrix of string in scilab's internal memory 
 * calling sequence 
 *     logic=creadchains('matrixname',size,string)
 * @param name__ character string; name of the scilab variable. 
 * @param indiceRow The row position
 * @param indiceCol The column position
 * @param itslen The length of the retrieved string
 * @param chai the string retrieved
 * @param name_len strlen of name (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadchains)  (char *name__, int *indiceRow, int *indiceCol, int *itslen, char *chai, unsigned long name_len, unsigned long chai_len);

/**
 * Write a string into the Scilab memory
 *
 * @param name__ name of the scilab variable
 * @param m length of the string itself
 * @param chai the string itself
 * @param name_len strlen of name__ (fortran needs it)
 * @param chai_len strlen of chai (fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritechain)  (char *name__, int *m, char *chai, unsigned long name_len, unsigned long chai_len);

/**
* Get length of strings by name
* @param[in] name_ name of the scilab variable
* @param[out] m number of rows of the matrix 
* @param[out] n number of columns of the matrix 
* @return array of length or NULL
*/
int *GetLengthStringMatrixByName(char *name_, int *m, int *n);

/**
 * Get pointer on a named matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(matptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named complex matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatcptr)  (char *name__, int *m, int *n, int *lp, unsigned long name_len);

/**
 * Get pointer on a named string matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param ix
 * @param j
 * @param lp
 * @param nlr
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatsptr)  (char *name__, int *m, int *n, int *ix, int *j, int *lp, int *nlr, unsigned long name_len);

/**
 * Read a boolean matrix in scilab's memory
 * calling sequence 
 *     logic=creadbmat('matrixname',m,n,scimat)
 * @example
 *    Amat is a real 2 x 3 scilab matrix 
 *    your subroutine should be as follows: 
 *    subroutine mysubr(...) 
 *    ... 
 * @code
 *    call creadbmat('Amat',m,n,scimat) 
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
 * @param scimat boolean matrix entries stored columnwise
 * @param name_len
 * @return if the operation successed (true) or not (false)
 */
int C2F(creadbmat)(char *namex, int *m, int *n, int *scimat, unsigned long name_len);

/**
 * cwritemat writes vector/matrix of boolean in scilab's memory
 * logic=cwritebmat('matrixname'//char(0),m,n,mat)
 * @code
 *	int A[]={1,0,0,1};   // Declare the matrix 
 * // NOTE that it is an array of int and not an array of double 
 *		int rowA=1, colA=4; // Size of the matrix 
 *	char variableName[]="A";
 *
 *	C2F(cwritebmat)(variableName, &rowA, &colA, A,strlen(variableName)); // Write it into Scilab's memory 
 * @endcode
 * @param name__ character string; name of the scilab variable ( null terMinated) 
 * @param m number of rows 
 * @param n number of columns 
 * @param mat boolean matrix entries stored columnwise in Scilab object
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
 */
int C2F(cwritebmat)(char *namex, int *m, int *n, int *mat, unsigned long name_len);

/**
 * Get pointer on a named boolean matrix 
 * @param namex the name of the Scilab variable
 * @param m number of rows of the matrix 
 * @param n number of columns of the matrix 
 * @param lp
 * @param name_len strlen of name__ (Fortran needs it)
 * @return if the operation successed (true) or not (false)
*/
int C2F(cmatbptr)(char *namex, int *m,int *n,int *lp, unsigned long name_len);

int C2F(str2name)  (char *name__, int *id, unsigned long name_len);
int C2F(objptr)  (char *name__, int *lp, int *fin, unsigned long name_len);
int C2F(putvar) (int *number, char *namex, unsigned long name_len );


/**
 * Returns length of a "chain variable" in scilab
 * example :
 * in scilab --> str = "abcdefghijklmnopqrstuvwxyz";
 * in C getlengthchain("str") returns 26
 * @param namex
 * @return  -1 if error
*/ 
int getlengthchain(char *namex);


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
int	iAllocMatrixOfDoubleComplex(int _iNewVal, int _iRows, int _iCols, double **_pdblRealData, double **_pdblImgData);

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

//Internal fonctions to retrieve variables information from Address ( old "il" )
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
void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);

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
#endif 
