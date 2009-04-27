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

#ifndef __VARIABLE_API__
#define __VARIABLE_API__

#include "machine.h"
#include "doublecomplex.h"

/* generics functions */

int getVarType(int* _piAddr);
int isVarComplex(int* _piAddr);

/**
 * Get memory address of a variable from the variable number
 * @param[in] _iVar variable number
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
int getVarAddressFromNumber(int _iVar, int** _piAddress);


/**
 * Get memory address of a variable from the variable name
 * @param[in] _pstName variable name
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
int getVarAddressFromName(char* _pstName, int** _piAddress);

/**
 * Get variable type
 * @param[in] _piAddress variable address
 * @return scilab variable type ( sci_matrix, sci_strings, ... )
 */
int getVarType(int* _piAddress);

/**
 * Get complex information
 * @param[in] _piAddress variable address
 * @return if complex 1 otherwise 0
 */
int isVarComplex(int* _piAddress);

/**
 * Get variable dimension
 * @param[in] _piAddress variable address
 * @param[out] _piRows Number of rows
 * @param[out] _piCols Number of cols
 * @return if the operation successed (0) or not ( !0 )
 */
int getVarDimension(int* _piAddress, int* _piRows, int* _piCols);


/*******************************/
/*   double matrix functions   */
/*******************************/
/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
int getComplexMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblZ return pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
int getComplexZMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int allocMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int allocComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedComplexMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length 
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMatrixOfDouble(char* _pstName, int _iNameLen, int* _iRows, int* _iCols, double* _pdblReal);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[in] _iNameLen variable name length 
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedComplexMatrixOfDouble(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);



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



/*****************************/
/*   string matrix functions   */
/*****************************/

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return strings length
 * @param[out] _pstStrings return array of char
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfString(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piLength return cumulative strings length
 * @param[out] _pstStrings return array of char
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfStringCumulativeLength(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char* _pstStrings);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMatrixOfString(char* _pstName, int _iNameLen, int _iRows, int _iCols, char** _pstStrings);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iNamelen variable name length
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pstStrings array of strings ( null terminated
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMatrixOfString(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings);

#endif /* __VARIABLE_API__ */