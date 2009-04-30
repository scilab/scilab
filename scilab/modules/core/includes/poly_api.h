/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __POLY_API__
#define __POLY_API__

/*****************************/
/*   poly matrix functions   */
/*****************************/

/**
 * Get polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstVarName return the variable name of polynomials
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piDegree return an array of polynomials degrees
 * @param[out] _pdblReal return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfPoly (int* _piAddress, char** _pstVarName, int* _piRows, int* _piCols, int** _piDegree, double** _pdblReal);

/**
 * Get complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstVarName return the variable name of polynomials
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piDegree return an array of polynomials degrees
 * @param[out] _pdblReal return polynomials coefficients
 * @param[out] _pdblImg return polynomials coefficients
 * @return if the operation successed (0) or not ( !0 )
 */
int getComplexMatrixOfPoly (int* _piAddress, char** _pstVarName, int* _piRows, int* _piCols, int** _piDegree, double** _pdblReal, double** _pdblImg);

/**
 * Get complex polynomial variable data
 * @param[in] _piAddress variable address
 * @param[out] _pstVarName return the variable name of polynomials
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piDegree return an array of polynomials degrees
 * @param[out] _pdblZ return pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
int getComplexZMatrixOfPoly (int* _piAddress, char** _pstVarName, int* _piRows, int* _piCols, int** _piDegree, double** _pdblZ);


/**
 * Get complex polynomial variable data
 * @param[in] _pstVarName variable name of polynomials
 * @param[in] _iRows number of row 
 * @param[in] _iCols number of column
 * @param[in] _piDegree array of polynomials degrees
 * @param[out] _pdblReal return pointer on coefficients
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int allocMatrixOfPoly(char* _pstVarName, int _iRows, int _iCols, int* _piDegree, double** _pdblReal, int** _piAddress);

/**
 * Get complex polynomial variable data
 * @param[in] _pstVarName variable name of polynomials
 * @param[in] _iRows number of row 
 * @param[in] _iCols number of column
 * @param[in] _piDegree array of polynomials degrees
 * @param[out] _pdblReal return pointer on real coefficients
 * @param[out] _pdblImg return pointer on img coefficients
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int allocComplexMatrixOfPoly(char* _pstVarName, int _iRows, int _iCols, int* _piDegree, double** _pdblReal, double** _pdblImg, int** _piAddress);

/**
 * Get complex polynomial variable data
 * @param[in] _pstVarName variable name of polynomials
 * @param[in] _iRows number of row 
 * @param[in] _iCols number of column
 * @param[in] _piDegree array of polynomials degrees
 * @param[in] _pdblReal pointer on coefficients
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int setMatrixOfPoly(char* _pstVarName, int _iRows, int _iCols, int* _piDegree, double* _pdblReal, int** _piAddress);

/**
 * Get complex polynomial variable data
 * @param[in] _pstVarName variable name of polynomials
 * @param[in] _iRows number of row 
 * @param[in] _iCols number of column
 * @param[in] _piDegree array of polynomials degrees
 * @param[in] _pdblReal pointer on real coefficients
 * @param[in] _pdblImg pointer on img coefficients
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int setComplexMatrixOfPoly(char* _pstVarName, int _iRows, int _iCols, int* _piDegree, double* _pdblReal, double* _pdblImg, int** _piAddress);

#endif /* __POLY_API__ */