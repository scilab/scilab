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
} StrErr;

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
 * Get memory address of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress);

/**
 * Get memory address of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _pstName variable name
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getVarNameFromPosition(void* _pvCtx, int _iVar, char* _pstName);

/**
 * Get memory address of a variable from the variable name
 * @param[in] _pstName variable name
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getVarAddressFromName(void* _pvCtx, char* _pstName, int** _piAddress);

/**
 * Get variable type
 * @param[in] _piAddress variable address
 * @param[out] returns _piType variable type
 * @return scilab variable type ( sci_matrix, sci_strings, ... )
 */
StrErr getVarType(void* _pvCtx, int* _piAddress, int* _piType);

/**
* Get variable type  from the variable name
* @param[in] _piAddress variable address
* @return scilab variable type ( sci_matrix, sci_strings, ... )
*/
StrErr getNamedVarType(void* _pvCtx, char* _pstName, int* _piType);

/**
 * Get complex information
 * @param[in] _piAddress variable address
 * @return if complex 1 otherwise 0
 */
int isVarComplex(void* _pvCtx, int* _piAddress);

/**
* Get named complex information
* @param[in] _pstName variable name
* @return if complex 1 otherwise 0
*/
int isNamedVarComplex(void* _pvCtx, char *_pstName);

/**
 * Get variable dimension
 * @param[in] _piAddress variable address
 * @param[out] _piRows Number of rows
 * @param[out] _piCols Number of cols
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getVarDimension(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols);

/**
* Get named variable dimension
* @param[in] _pstName variable name
* @param[out] _piRows Number of rows
* @param[out] _piCols Number of cols
* @return if the operation successed (0) or not ( !0 )
*/
StrErr getNamedVarDimension(void* _pvCtx, char *_pstName, int* _piRows, int* _piCols);

/**
 * check if a variable is a matrix form ( row x col )
 * @param[in] _piAddress variable address
 * @return if matrix form type variable 1 otherwise 0
 */
int isVarMatrixType(void* _pvCtx, int* _piAddress);

/**
* check if a named variable is a matrix form ( row x col )
* @param[in] _pstName variable name
* @return if matrix form type variable 1 otherwise 0
*/
int isNamedVarMatrixType(char *_pstName);

/**
 * get process mode from input variable
 * @param[in] _iPos variable position in function call
 * @param[in] _piAddRef variable address
 * @param[out] _piMode return process mode ( 0 -> All, 1 -> Row, 2 -> Col )
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getProcessMode(void* _pvCtx, int _iPos, int* _piAddRef, int *_piMode);

/**
 * get dimension for variable, extract value from a single value
 * @param[in] _piAddress variable address ( double or int variable )
 * @param[out] _piVal return value
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getDimFromVar(void* _pvCtx, int* _piAddress, int* _piVal);

/**
* get dimension for a named variable, extract value from a single value
* @param[in] _pstName variable name
* @param[out] _piVal return value
* @return if the operation successed (0) or not ( !0 )
*/
StrErr getDimFromNamedVar(void* _pvCtx, char* _pstName, int* _piVal);

/**
* Get Rhs value from variable Address
* @param[in] _piAddress varaible address
* @return rhs value of the variable, if failed returns 0
*/
int getRhsFromAddress(void* _pvCtx, int* _piAddress);

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_API__ */
