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

#ifndef __POLY_API__
#define __POLY_API__

#ifdef __cplusplus
extern "C" {
#endif


/*****************************/
/*   poly matrix functions   */
/*****************************/

/**
 * Get polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstVarName return variable name of polynomials
 * @param[out] _piVarNameLen returns length of _pstVarName
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen);

/**
 * Get polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return an array of polynomials coefficients
 * @param[out] _pdblReal return polynomials coefficients
 * @param[out] _pdblImg return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get polynomial named variable data
 * @param[in] _iVar variable number
 * @param[in] _pstVarName Variable Name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get complex polynomial named variable data
 * @param[in] _iVar variable number
 * @param[in] _pstVarName Variable Name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Create polynomial named variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr createNamedMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * Create complex polynomial named variable
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piNbCoef array of polynomials coefficients
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr createNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return polynomials coefficients
 * @param[out] _pdblReal return real data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr readNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piNbCoef return polynomials coefficients
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
 
SciErr readNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/* shortcut functions */

/**
 * check if the variable type is a poly
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isPolyType(void* _pvCtx, int* _piAddress);

/**
 * check if the variable type is a poly
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedPolyType(void* _pvCtx, char* _pstName);

/**
 * Get single poly variable data
 * @param[in] _piAddress variable address
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal);

/**
 * Get single complex poly variable data
 * @param[in] _piAddress variable address
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedSingleComplexPoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get named single poly variable data
 * @param[in] _pstName variable name
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedSinglePoly(void* _pvCtx, char* _pstName, int* _piNbCoef, double** _pdblReal);

/**
 * Get named single complex poly variable data
 * @param[in] _pstName variable name
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedSingleComplexPoly(void* _pvCtx, char* _pstName, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get matrix of poly variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of col
 * @param[out] _piNbCoef return polynomials coefficients number
 * @param[out] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal);

/**
 * Get matrix of complex poly variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of col
 * @param[out] _piNbCoef return polynomials coefficients number
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on imgaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedMatrixOfComplexPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);

/**
 * Get named matrix of poly variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of col
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal);

/**
 * Get named matrix of complex poly variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of col
 * @param[out] _piNbCoef return polynomial coefficient number
 * @param[out] _pdblReal pointer on real data
 * @param[out] _pdblImg pointer on imaginary data
 * @return if the operation successed (0) or not ( !0 )
 */
int getAllocatedNamedMatrixOfComplexPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pdblReal single polynom real data 
 */
void freeAllocatedSinglePoly(double* _pdblReal);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _pdblReal single polynom real data
 * @param[in] _pdblImg single polynom imaginary data
 */
void freeAllocatedSingleComplexPoly(double* _pdblReal, double* _pdblImg);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows row count
 * @param[in] _iCols column count
 * @param[in] _piNbCoef polynomial coefficient number
 * @param[in] _pdblReal matrix of polynom real data 
 */
void freeAllocatedMatrixOfPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * free data allocated by shortcut functions ( named or not )
 * @param[in] _iRows row count
 * @param[in] _iCols column count
 * @param[in] _piNbCoef polynomial coefficient number
 * @param[in] _pdblReal matrix of polynom real data
 * @param[in] _pdblImg matrix of polynom imaginary data
 */
void freeAllocatedMatrixOfComplexPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

#ifdef __cplusplus
}
#endif
#endif /* __POLY_API__ */
