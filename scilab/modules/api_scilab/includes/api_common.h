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

#include "dynlib_api_scilab.h"

/* generics functions */

/**
 * Get memory address of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getVarAddressFromPosition(int _iVar, int** _piAddress);

/**
 * Get memory address of a variable from the variable position
 * @param[in] _iVar variable number
 * @param[out] _pstName variable name
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getVarNameFromPosition(int _iVar, char* _pstName);
/**
 * Get memory address of a variable from the variable name
 * @param[in] _pstName variable name
 * @param[out] _piAddress return variable address
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getVarAddressFromName(char* _pstName, int** _piAddress);

/**
 * Get variable type
 * @param[in] _piAddress variable address
 * @return scilab variable type ( sci_matrix, sci_strings, ... )
 */
API_SCILAB_IMPEXP int getVarType(int* _piAddress);

/**
* Get variable type  from the variable name
* @param[in] _piAddress variable address
* @return scilab variable type ( sci_matrix, sci_strings, ... )
*/
API_SCILAB_IMPEXP int getNamedVarType(char* _pstName);

/**
 * Get complex information
 * @param[in] _piAddress variable address
 * @return if complex 1 otherwise 0
 */
API_SCILAB_IMPEXP int isVarComplex(int* _piAddress);

/**
* Get named complex information
* @param[in] _pstName variable name
* @return if complex 1 otherwise 0
*/
API_SCILAB_IMPEXP int isNamedVarComplex(char *_pstName);

/**
 * Get variable dimension
 * @param[in] _piAddress variable address
 * @param[out] _piRows Number of rows
 * @param[out] _piCols Number of cols
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getVarDimension(int* _piAddress, int* _piRows, int* _piCols);

/**
* Get named variable dimension
* @param[in] _pstName variable name
* @param[out] _piRows Number of rows
* @param[out] _piCols Number of cols
* @return if the operation successed (0) or not ( !0 )
*/
API_SCILAB_IMPEXP int getNamedVarDimension(char *_pstName, int* _piRows, int* _piCols);

/**
 * check if a variable is a matrix form ( row x col )
 * @param[in] _piAddress variable address
 * @return if matrix form type variable 1 otherwise 0
 */
API_SCILAB_IMPEXP int isVarMatrixType(int* _piAddress);

/**
* check if a named variable is a matrix form ( row x col )
* @param[in] _pstName variable name
* @return if matrix form type variable 1 otherwise 0
*/
API_SCILAB_IMPEXP int isNamedVarMatrixType(char *_pstName);

/**
 * get process mode from input variable
 * @param[in] _iPos variable position in function call
 * @param[in] _piAddRef variable address
 * @param[out] _piMode return process mode ( 0 -> All, 1 -> Row, 2 -> Col )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getProcessMode(int _iPos, int* _piAddRef, int *_piMode);

/**
 * get dimension for variable, extract value from a single value
 * @param[in] _piAddress variable address ( double or int variable )
 * @param[out] _piVal return value
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getDimFromVar(int* _piAddress, int* _piVal);

/**
* get dimension for a named variable, extract value from a single value
* @param[in] _pstName variable name
* @param[out] _piVal return value
* @return if the operation successed (0) or not ( !0 )
*/
API_SCILAB_IMPEXP int getDimFromNamedVar(char* _pstName, int* _piVal);

#ifdef __cplusplus
}
#endif
#endif /* __COMMON_API__ */
