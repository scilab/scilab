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

#ifndef __INT_API__
#define __INT_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_int.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* These values are also used in:
 * modules/core/includes/sci_types.h */
#define SCI_INT8	1
#define SCI_INT16	2
#define SCI_INT32	4
#define SCI_INT64	8

#define SCI_UINT8	11
#define SCI_UINT16	12
#define SCI_UINT32	14
#define SCI_UINT64	18

/********************************/
/*   integer matrix functions   */
/********************************/

/**
 * Get integer precision ( SCI_xINTx )
 * @param[in] _piAddress variable address
 * @param[out] _piPrecison return integer precision ( SCI_xINTx )
 */
SciErr getMatrixOfIntegerPrecision(void* _pvCtx, int* _piAddress, int* _piPrecision);

/**
 * Get 8-bit integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pcData8 return pointer to 8-bit integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char** _pcData8);

/**
 * Get 8-bit unsigned integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pucData8 return pointer to 8-bit unsigned integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned char** _pucData8);

/**
 * Get 16-bit integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _psData16 return pointer to 16-bit integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, short** _psData16);

/**
 * Get 16-bit unsigned integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pusData16 return pointer to 16-bit unsigned integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned short** _pusData16);

/**
 * Get 32-bit integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piData32 return pointer to 32-bit integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piData32);

/**
 * Get 32-bit unsigned integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _puiData32 return pointer to 32-bit unsigned integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned int** _puiData32);


#ifdef __SCILAB_INT64__

/**
 * Get 64-bit integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pllData64 return pointer to 64-bit integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllData64);

/**
 * Get 64-bit unsigned integer variable
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pullData64 return pointer to 64-bit unsigned integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr getMatrixOfUnsignedInteger64(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, unsigned long long** _pullData64);

#endif

/**
 * Create 8-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pcData8 array of 8-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* _pcData8);

/**
 * Create 8-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pucData8 array of 8-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned char* _pucData8);

/**
 * Create 16-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _psData16 array of 16-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const short* _psData16);

/**
 * Create 16-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pusData16 array of 16-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned short* _pusData16);

/**
 * Create 32-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piData32 array of 32-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piData32);

/**
 * Create 32-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _puiData32 array of 32-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned int* _puiData32);

#ifdef __SCILAB_INT64__

/**
 * Create 64-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pllData64 array of 64-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, const long long* _pllData64);

/**
 * Create 64-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pullData64 array of 64-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, const unsigned long long* _pullData64);

#endif

/**
 * Create named 8-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pcData8 array of 8-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger8(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const char* _pcData8);

/**
 * Create named 8-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pucData8 array of 8-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfUnsignedInteger8(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned char* _pucData8);

/**
 * Create named 16-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _psData16 array of 16-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger16(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const short* _psData16);

/**
 * Create named 16-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pusData16 array of 16-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfUnsignedInteger16(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned short* _pusData16);

/**
 * Create named 32-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _piData32 array of 32-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger32(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const int* _piData32);

/**
 * Create named 32-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _puiData32 array of 32-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfUnsignedInteger32(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned int* _puiData32);

#ifdef __SCILAB_INT64__

/**
 * Create named 64-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pllData64 array of 64-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfInteger64(void* _pvCtx, cosnt char* _pstName, int _iRows, int _iCols, const long long* _pllData64);

/**
 * Create named 64-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[in] _pullData64 array of 64-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr createNamedMatrixOfUnsignedInteger64(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const unsigned long long* _pullData64);

#endif

/**
 * Allocate 8-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pcData8 return array of 8-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pcData8);

/**
 * Allocate 8-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pucData8 return array of 8-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfUnsignedInteger8(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned char** _pucData8);

/**
 * Allocate 16-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _psData16 return array of 16-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, short** _psData16);

/**
 * Allocate 16-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pusData16 return array of 16-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfUnsignedInteger16(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned short** _pusData16);

/**
 * Allocate 32-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _piData32 return array of 32-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piData32);

/**
 * Allocate 32-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _puiData32 return array of 32-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfUnsignedInteger32(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned int** _puiData32);

#ifdef __SCILAB_INT64__

/**
 * Allocate 64-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pllData64 return array of 64-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllData64);

/**
 * Allocate 64-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _iRows number of rows
 * @param[in] _iCols number of columns
 * @param[out] _pullData64 return array of 64-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr allocMatrixOfUnsignedInteger64(void* _pvCtx, int _iVar, int _iRows, int _iCols, unsigned long long** _pullData64);

#endif

/**
 * Get integer precision of a named variable ( SCI_xINTx )
 * @param[in] _pstName variable name
 * @param[out] _piPrecison return integer precision ( SCI_xINTx )
 */
SciErr getNamedMatrixOfIntegerPrecision(void* _pvCtx, const char* _pstName, int* _piPrecision);

/**
 * Read named 8-bit integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pcData8 return array of 8-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger8(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, char* _pcData8);

/**
 * Read named 8-bit unsigned integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pucData8 return array of 8-bit unsigned integer
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfUnsignedInteger8(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned char* _pucData8);

/**
 * Read named 16-bit integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _psData16 return array of 16-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger16(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, short* _psData16);

/**
 * Read named 16-bit unsigned integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pusData16 return array of 16-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfUnsignedInteger16(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned short* _pusData16);

/**
 * Read named 32-bit integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _piData32 return array of 32-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger32(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piData32);

/**
 * Read named 32-bit unsigned integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _puiData32 return array of 32-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfUnsignedInteger32(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned int* _puiData32);

#ifdef __SCILAB_INT64__

/**
 * Read named 64-bit integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pllData64 return array of 64-bit integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfInteger64(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, long long* _pllData64);

/**
 * Read named 64-bit unsigned integer variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of rows
 * @param[out] _piCols return number of columns
 * @param[out] _pullData64 return array of 64-bit unsigned integers
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
SciErr readNamedMatrixOfUnsignedInteger64(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, unsigned long long* _pullData64);

#endif

/* shortcut functions */

/**
 * Check if the type of a variable is integer
 * @param[in] _piAddress variable address
 * @return 1 for true and 0 for false
 */
int isIntegerType(void* _pvCtx, int* _piAddress);

/**
 * Check if the type of a named variable is integer
 * @param[in] _pstName variable name
 * @return 1 for true and 0 for false
 */
int isNamedIntegerType(void* _pvCtx, const char* _pstName);

/**
 * Get scalar 8-bit integer value
 * @param[in] _piAddress variable address
 * @param[out] _pcData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarInteger8(void* _pvCtx, int* _piAddress, char* _pcData);

/**
 * Get scalar 8-bit unsigned integer value
 * @param[in] _piAddress variable address
 * @param[out] _pucData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarUnsignedInteger8(void* _pvCtx, int* _piAddress, unsigned char* _pucData);

/**
 * Get scalar 16-bit integer value
 * @param[in] _piAddress variable address
 * @param[out] _psData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarInteger16(void* _pvCtx, int* _piAddress, short* _psData);

/**
 * Get scalar 16-bit unsigned integer value
 * @param[in] _piAddress variable address
 * @param[out] _pusData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarUnsignedInteger16(void* _pvCtx, int* _piAddress, unsigned short* _pusData);

/**
 * Get scalar 32-bit integer value
 * @param[in] _piAddress variable address
 * @param[out] _piData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarInteger32(void* _pvCtx, int* _piAddress, int* _piData);

/**
 * Get scalar 32-bit unsigned integer value
 * @param[in] _piAddress variable address
 * @param[out] _puiData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarUnsignedInteger32(void* _pvCtx, int* _piAddress, unsigned int* _puiData);

#ifdef __SCILAB_INT64__

/**
 * Get scalar 64-bit integer value
 * @param[in] _piAddress variable address
 * @param[out] _pllData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarInteger64(void* _pvCtx, int* _piAddress, long long* _pllData);

/**
 * Get scalar 64-bit unsigned integer value
 * @param[in] _piAddress variable address
 * @param[out] _pullData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getScalarUnsignedInteger64(void* _pvCtx, int* _piAddress, unsigned long long* _pullData);

#endif

/**
 * Get named scalar 8-bit integer value
 * @param[in] _pstName variable name
 * @param[out] _pcData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarInteger8(void* _pvCtx, const char* _pstName, char* _pcData);

/**
 * Get named scalar 8-bit unsigned integer value
 * @param[in] _pstName variable name
 * @param[out] _pucData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarUnsignedInteger8(void* _pvCtx, const char* _pstName, unsigned char* _pucData);

/**
 * Get named scalar 16-bit integer value
 * @param[in] _pstName variable name
 * @param[out] _psData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarInteger16(void* _pvCtx, const char* _pstName, short* _psData);

/**
 * Get named scalar 16-bit unsigned integer value
 * @param[in] _pstName variable name
 * @param[out] _pusData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarUnsignedInteger16(void* _pvCtx, const char* _pstName, unsigned short* _pusData);

/**
 * Get named scalar 32-bit integer value
 * @param[in] _pstName variable name
 * @param[out] _piData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarInteger32(void* _pvCtx, const char* _pstName, int* _piData);

/**
 * Get named scalar 32-bit unsigned integer value
 * @param[in] _pstName variable name
 * @param[out] _puiData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarUnsignedInteger32(void* _pvCtx, const char* _pstName, unsigned int* _puiData);

#ifdef __SCILAB_INT64__

/**
 * Get named scalar 64-bit integer value
 * @param[in] _pstName variable name
 * @param[out] _pllData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarInteger64(void* _pvCtx, const char* _pstName, long long* _pllData);

/**
 * Get named scalar 64-bit unsigned integer value
 * @param[in] _pstName variable name
 * @param[out] _pullData return integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getNamedScalarUnsignedInteger64(void* _pvCtx, const char* _pstName, unsigned long long* _pullData);

#endif

/**
 * Create scalar 8-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _cData 8-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarInteger8(void* _pvCtx, int _iVar, char _cData);

/**
 * Create scalar 8-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _ucData 8-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarUnsignedInteger8(void* _pvCtx, int _iVar, unsigned char _ucData);

/**
 * Create scalar 16-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _sData 16-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarInteger16(void* _pvCtx, int _iVar, short _sData);

/**
 * Create scalar 16-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _usData 16-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarUnsignedInteger16(void* _pvCtx, int _iVar, unsigned short _usData);

/**
 * Create scalar 32-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _iData 32-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarInteger32(void* _pvCtx, int _iVar, int _iData);

/**
 * Create scalar 32-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _uiData 32-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarUnsignedInteger32(void* _pvCtx, int _iVar, unsigned int _uiData);

#ifdef __SCILAB_INT64__

/**
 * Create scalar 64-bit integer variable
 * @param[in] _iVar variable number
 * @param[in] _llData 64-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarInteger64(void* _pvCtx, int _iVar, long long _llData);

/**
 * Create scalar 64-bit unsigned integer variable
 * @param[in] _iVar variable number
 * @param[in] _ullData 64-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createScalarUnsignedInteger64(void* _pvCtx, int _iVar, unsigned long long _ullData);

#endif

/**
 * Create named scalar 8-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _cData 8-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarInteger8(void* _pvCtx, const char* _pstName, char _cData);

/**
 * Create named scalar 8-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _ucData 8-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarUnsignedInteger8(void* _pvCtx, const char* _pstName, unsigned char _ucData);

/**
 * Create named scalar 16-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _sData 16-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarInteger16(void* _pvCtx, const char* _pstName, short _sData);

/**
 * Create named scalar 16-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _usData 16-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarUnsignedInteger16(void* _pvCtx, const char* _pstName, unsigned short _usData);

/**
 * Create named scalar 32-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _iData 32-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarInteger32(void* _pvCtx, const char* _pstName, int _iData);

/**
 * Create named scalar 32-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _uiData 32-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarUnsignedInteger32(void* _pvCtx, const char* _pstName, unsigned int _uiData);

#ifdef __SCILAB_INT64__

/**
 * Create named scalar 64-bit integer variable
 * @param[in] _pstName variable name
 * @param[in] _llData 64-bit integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarInteger64(void* _pvCtx, const char* _pstName, long long _llData);

/**
 * Create named scalar 64-bit unsigned integer variable
 * @param[in] _pstName variable name
 * @param[in] _ullData 64-bit unsigned integer value
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int createNamedScalarUnsignedInteger64(void* _pvCtx, const char* _pstName, unsigned long long _ullData);

#endif

#ifdef __cplusplus
}
#endif
#endif /* __INT_API__ */
