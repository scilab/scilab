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

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_poly.h. Include api_scilab.h instead.
#endif

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
 * @param[out] _piVarNameLen return length of _pstVarName
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen);

/**
 * Get polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return real coefficients
 * @param[out] _pdblImg return imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Create a polynomial variable
 * @param[in] _iVar variable number
 * @param[in] _pstVarName polynomial variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal);

/**
 * Create complex polynomial variable data
 * @param[in] _iVar variable number
 * @param[in] _pstVarName polynomial variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 * @param[in] _pdblImg pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

/**
 * Create polynomial named variable
 * @param[in] _pstName variable name
 * @param[in] _pstVarName polynomial variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr createNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal);

/**
 * Create complex polynomial named variable
 * @param[in] _pstName variable name
 * @param[in] _pstVarName polynomial variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 * @param[in] _pdblImg pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr createNamedComplexMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);

/**
 * Get named polynomial variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr readNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal);

/**
 * Get named complex polynomial variable
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return real coefficients
 * @param[out] _pdblImg return imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
 
SciErr readNamedComplexMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/* shortcut functions */

/**
 * Check if the variable type is polynomial
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isPolyType(void* _pvCtx, int* _piAddress);

/**
 * Check if the named variable type is polynomial
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedPolyType(void* _pvCtx, const char* _pstName);

/**
 * Get single polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piNbCoef return number of polynomial coefficients
 * @param[out] _pdblReal pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal);

/**
 * Get single complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piNbCoef return number of polynomial coefficients
 * @param[out] _pdblReal pointer to real coefficients
 * @param[out] _pdblImg pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedSingleComplexPoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get single named polynomial variable data
 * @param[in] _pstName variable name
 * @param[out] _piNbCoef return number of polynomial coefficients
 * @param[out] _pdblReal pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedNamedSinglePoly(void* _pvCtx, const char* _pstName, int* _piNbCoef, double** _pdblReal);

/**
 * Get single named complex polynomial variable data
 * @param[in] _pstName variable name
 * @param[out] _piNbCoef return number of polynomial coefficients
 * @param[out] _pdblReal pointer to real coefficients
 * @param[out] _pdblImg pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedNamedSingleComplexPoly(void* _pvCtx, const char* _pstName, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

/**
 * Get matrix of polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients
 * @param[out] _pdblReal return pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal);

/**
 * Get matrix of complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return pointer to real coefficients
 * @param[out] _pdblImg return pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedMatrixOfComplexPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);

/**
 * Get named matrix of polynomials variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return pointer to real coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal);

/**
 * Get named matrix of complex polynomials variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piNbCoef return number of polynomial coefficients for each element
 * @param[out] _pdblReal return pointer to real coefficients
 * @param[out] _pdblImg return pointer to imaginary coefficients
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getAllocatedNamedMatrixOfComplexPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);

/**
 * Free memory allocated for a single polynomial variable ( named or not )
 * @param[in] _pdblReal pointer to real coefficients
 */
void freeAllocatedSinglePoly(double* _pdblReal);

/**
 * Free memory allocated for a single complex polynomial variable ( named or not )
 * @param[in] _pdblReal pointer to real coefficients
 * @param[in] _pdblImg pointer to imaginary coefficients
 */
void freeAllocatedSingleComplexPoly(double* _pdblReal, double* _pdblImg);

/**
 * Free memory allocated for a polynomial matrix ( named or not )
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 */
void freeAllocatedMatrixOfPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal);

/**
 * Free memory allocated for a complex polynomial matrix ( named or not )
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piNbCoef number of polynomial coefficients for each element
 * @param[in] _pdblReal pointer to real coefficients
 * @param[in] _pdblImg pointer to imaginary coefficients
 */
void freeAllocatedMatrixOfComplexPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

#ifdef __cplusplus
}
#endif
#endif /* __POLY_API__ */
