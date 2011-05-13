/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
  * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __COMMON_API__
#define __COMMON_API__

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * The error management structure
	 *
	 */

#define MESSAGE_STACK_SIZE 5

typedef struct api_Err
{
	int iErr; /**< The error ID */
	int iMsgCount; /**< Error level */
	char* pstMsg[MESSAGE_STACK_SIZE]; /**< The error message */
} SciErr;

typedef struct api_Ctx
{
	char* pstName; /**< Function name */
} StrCtx, *pStrCtx;

#ifdef _MSC_VER
	#ifndef API_SCILAB_EXPORTS
		__declspec( dllimport ) StrCtx* pvApiCtx;
	#endif
#else
	extern StrCtx* pvApiCtx;
#endif

#include "api_error.h"
/* generics functions */

/**
 * Get the memory address of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _piAddress return variable address
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress);

/**
 * Get the name of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _pstName variable name
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getVarNameFromPosition(void* _pvCtx, int _iVar, char* _pstName);

/**
 * Get the memory address of a variable from the variable name
 * @param[in] _pstName variable name
 * @param[out] _piAddress return variable address
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getVarAddressFromName(void* _pvCtx, const char* _pstName, int** _piAddress);

/**
 * Get the variable type
 * @param[in] _piAddress variable address
 * @param[out] _piType return variable type ( sci_matrix, sci_strings, ... )
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getVarType(void* _pvCtx, int* _piAddress, int* _piType);

/**
 * Get the variable type from the variable name
 * @param[in] _pstName variable name
 * @param[out] _piType return variable type ( sci_matrix, sci_strings, ... )
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getNamedVarType(void* _pvCtx, const char* _pstName, int* _piType);

/**
 * Check if the variable type is complex double, complex sparse double or complex polynomial
 * @param[in] _piAddress variable address
 * @return if complex 1 otherwise 0
 */
int isVarComplex(void* _pvCtx, int* _piAddress);

/**
* Check if the named variable type is complex double, complex sparse double or complex polynomial
* @param[in] _pstName variable name
* @return if complex 1 otherwise 0
*/
int isNamedVarComplex(void* _pvCtx, const char *_pstName);

/**
 * Get variable dimensions
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of cols
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getVarDimension(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols);

/**
 * Get named variable dimensions
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of cols
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getNamedVarDimension(void* _pvCtx, const char *_pstName, int* _piRows, int* _piCols);

/**
 * Check if a variable is a matrix form ( row x col )
 * @param[in] _piAddress variable address
 * @return if matrix form type variable 1 otherwise 0
 */
int isVarMatrixType(void* _pvCtx, int* _piAddress);

/**
 * Check if a named variable is a matrix form ( row x col )
 * @param[in] _pstName variable name
 * @return if matrix form type variable 1 otherwise 0
 */
int isNamedVarMatrixType(void* _pvCtx, const char *_pstName);

/**
 * Get process mode from input variable
 * @param[in] _iPos variable position in function call
 * @param[in] _piAddRef variable address
 * @param[out] _piMode return process mode ( 0 -> All, 1 -> Row, 2 -> Col )
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getProcessMode(void* _pvCtx, int _iPos, int* _piAddRef, int *_piMode);

/**
 * Get dimension for variable, extract value from a single value
 * @param[in] _piAddress variable address ( double or int variable )
 * @param[out] _piVal return value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getDimFromVar(void* _pvCtx, int* _piAddress, int* _piVal);

/**
 * Get dimension for a named variable, extract value from a single value
 * @param[in] _pstName variable name
 * @param[out] _piVal return value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getDimFromNamedVar(void* _pvCtx, const char* _pstName, int* _piVal);

/**
 * Get Rhs value from variable address
 * @param[in] _piAddress varaible address
 * @return rhs value of the variable, if failed returns 0
 */
int getRhsFromAddress(void* _pvCtx, int* _piAddress);

/**
 * Check if a variable is a row vector
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isRowVector(void* _pvCtx, int* _piAddress);

/**
 * Check if a variable is a column vector
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isColumnVector(void* _pvCtx, int* _piAddress);

/**
 * Check if a variable is a vector ( row or column )
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isVector(void* _pvCtx, int* _piAddress);

/**
 * Check if a variable is a scalar
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isScalar(void* _pvCtx, int* _piAddress);

/**
 * Check if a variable is a square matrix
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isSquareMatrix(void* _pvCtx, int* _piAddress);

/**
 * Check matrix dimension
 * @param[in] _piAddress variable address
 * @param[in] _iRows accepted number of rows ( if -1, don't check )
 * @param[in] _iCols accepted number of columns ( if -1, don't check )
 * @return 1 for true and 0 for false
 */
int checkVarDimension(void* _pvCtx, int* _piAddress, int _iRows, int _iCols);

/**
 * Check if a matrix is empty
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isEmptyMatrix(void* _pvCtx, int* _piAddress);

/**
 * Create an empty matrix
 * @param[in] _iVar variable number
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createEmptyMatrix(void* _pvCtx, int _iVar);

/**
 * Create a named empty matrix
 * @param[in] _pstName variable name
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedEmptyMatrix(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable exists
 * @param[in] _pstName variable name
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int isNamedVarExist(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is a row vector
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedRowVector(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is a column vector
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedColumnVector(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is a vector ( row or column )
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedVector(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is a scalar
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedScalar(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is a square matrix
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedSquareMatrix(void* _pvCtx, const char* _pstName);

/**
 * Check if a named variable is an empty matrix
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedEmptyMatrix(void* _pvCtx, const char* _pstName);

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_API__ */
