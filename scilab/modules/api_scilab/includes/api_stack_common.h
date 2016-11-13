/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
  * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __STACK_COMMON_API__
#define __STACK_COMMON_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_common.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "string.h"
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

#if SCI_VERSION_MAJOR < 6
typedef struct api_Ctx
{
    char* pstName; /**< Function name */
} StrCtx, *pStrCtx;
#endif

//#if SCI_VERSION_MAJOR < 6
//    #ifdef _MSC_VER
//     #ifndef API_SCILAB_EXPORTS
//         __declspec( dllimport ) StrCtx* pvApiCtx;
//        #else
//        extern StrCtx* pvApiCtx;
//     #endif
//    #endif
//#endif

#include "api_scilab.h"
/* generics functions */

/*Rhs*/
int* getNbInputArgument(void* _pvCtx);
#define nbInputArgument(PVCTX) (*getNbInputArgument(PVCTX))

#ifdef Rhs
#undef Rhs
#endif
#define Rhs (*getNbInputArgument(pvApiCtx))

/*Lhs*/
int* getNbOutputArgument(void* _pvCtx);
#define nbOutputArgument(PVCTX) (*getNbOutputArgument(PVCTX))
#ifdef Lhs
#undef Lhs
#endif
#define Lhs (*getNbOutputArgument(pvApiCtx))

/*PutLhs*/
int* assignOutputVariable(void* _pvCtx, int _iVal);
#define AssignOutputVariable(PVCTX, x) (*assignOutputVariable(PVCTX, x))
#ifdef LhsVar
#undef LhsVar
#endif
#define LhsVar(x) (*assignOutputVariable(pvApiCtx, x))

/*PutLhsVar*/
int returnArguments(void* _pvCtx);
#define ReturnArguments(PVCTX) if (! returnArguments(PVCTX)) { return 0; }
#ifdef PutLhsVar
#undef PutLhsVar
#endif
#define PutLhsVar() if (! returnArguments(pvApiCtx)) { return 0; }

/**
 * Check input argument count
 * @param[in] _iMin min value
 * @param[in] _iMax : max value
 * @return if _iMin >= rhs >= _iMax
 */
int checkInputArgument(void* _pvCtx, int _iMin, int _iMax);
#define CheckInputArgument(ctx, min, max) \
    if(checkInputArgument(ctx, min, max) == 0) \
    { \
        return 0; \
    }
#ifdef CheckRhs
#undef CheckRhs
#endif
#define CheckRhs(min, max) \
    if(checkInputArgument(pvApiCtx, min, max) == 0) \
    { \
        return 0; \
    }
/**
 * Check input argument count
 * @param[in] _iMin min value
 * @return if rhs >= _iMin
 */
int checkInputArgumentAtLeast(void* _pvCtx, int _iMin);
#define CheckInputArgumentAtLeast(ctx, min) \
    if(checkInputArgumentAtLeast(ctx, min) == 0) \
    { \
        return 0; \
    }

/**
 * Check input argument count
 * @param[in] _iMax : max value
 * @return if rhs <= max
 */
int checkInputArgumentAtMost(void* _pvCtx, int _iMax);
#define CheckInputArgumentAtMost(ctx, max) \
    if(checkInputArgumentAtMost(ctx, max) == 0) \
    { \
        return 0; \
    }

/**
 * Check output argument count
 * @param[in] _iMin min value
 * @param[in] _iMax : max value
 * @return if _iMin >= lhs >= _iMax
 */
int checkOutputArgument(void* _pvCtx, int _iMin, int _iMax);
#define CheckOutputArgument(ctx, min, max) \
    if(checkOutputArgument(ctx, min, max) == 0) \
    { \
        return 0; \
    }
#ifdef CheckLhs
#undef CheckLhs
#endif
#define CheckLhs(min, max) \
    if(checkOutputArgument(pvApiCtx, min, max) == 0) \
    { \
        return 0; \
    }

/**
 * Check output argument count
 * @param[in] _iMin min value
 * @return if lhs >= _iMin
 */
int checkOutputArgumentAtLeast(void* _pvCtx, int _iMin);
#define CheckOutputArgumentAtLeast(ctx, min) \
    if(checkOutputArgumentAtLeast(ctx, min) == 0) \
    { \
        return 0; \
    }

/**
 * Check output argument count
 * @param[in] _iMax : max value
 * @return if lhs <= max
 */
int checkOutputArgumentAtMost(void* _pvCtx, int _iMax);
#define CheckOutputArgumentAtMost(ctx, max) \
    if(checkOutputArgumentAtMost(ctx, max) == 0) \
    { \
        return 0; \
    }

int callOverloadFunction(void* _pvCtx, int _iVar, char* _pstName, unsigned int _iNameLen);
#define CallOverloadFunction(x) callOverloadFunction(pvApiCtx, x, fname, strlen(fname))
#ifdef OverLoad
#undef OverLoad
#endif
#define OverLoad(x) callOverloadFunction(pvApiCtx, x, fname, (unsigned int)strlen(fname))

int callScilabFunction(void* _pvCtx, const char* _pstName, int _iStart, int _iLhs, int _iRhs);

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
* Check if the variable type is Cell
* @param[in] _piAddress variable address
* @return if complex 1 otherwise 0
*/
int isCell(void *_pvCtx, int *_piAddress);

/**
* Check if the variable type is Struct
* @param[in] _piAddress variable address
* @return if complex 1 otherwise 0
*/
int isStruct(void *_pvCtx, int *_piAddress);

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
 * @param[in] _piAddress variable address
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
 * Check if type of variable
 * @param[in] _iVar variable position
 * @return 1 for true and 0 for false
 */
int getInputArgumentType(void* _pvCtx, int _iVar);

/**
 * Check if type of variable
 * @param[in] _iVar variable position
 * @param[in] _iType variable type to check
 * @return 1 for true and 0 for false
 */
int checkInputArgumentType(void* _pvCtx, int _iVar, int _iType);

/**
* Reshape variable with new size
* @param[in] _iDimsArray array of dimensions
* @param[in] _iDims number of dimension
* @return if the operation succeeded ( 0 ) or not ( !0 )
*/
SciErr reshapeArray(void* _pvCtx, int* _piAddress, int* _iDimsArray, int _iDims);

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

/**
 * Delete a variable for stack
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int deleteNamedVariable(void* _pvCtx, const char* _pstName);

/**
 * increase ref for user_data handle property
 * @param[in] _piAddress variable address
 * @return 1 for successand 0 for failure
 */
int increaseValRef(void* _pvCtx, int* _piAddress);

/**
 * decrease ref for user_data handle property
 * @param[in] _piAddress variable address
 * @return 1 for successand 0 for failure
 */
int decreaseValRef(void* _pvCtx, int* _piAddress);

/**
 * Initialize SciErr
 */
SciErr sciErrInit();

/**
 * Clean & reset a SciErr
 */
void sciErrClean(SciErr* _psciErr);

#ifdef __cplusplus
}
#endif
#endif /* __STACK_COMMON_API__ */
