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

#define SCI_INT8		1
#define SCI_INT16		2
#define SCI_INT32		4
#define SCI_INT64		8

/********************************/
/*   integer matrix functions   */
/********************************/

/**
 * Get integer precision ( 8, 16, 32, 64 )
 * @param[in] _piAddress variable address
 * @param[out] _piPrecison return integer precision ( 8, 1-, 32, 64 )
 */
int getMatrixOfIntegerPrecision(int* _piAddress, int* _piPrecision);

/**
 * Get integer 8 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData8 return pointer on 8 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfInteger8(int* _piAddress, int* _piRows, int* _piCols, char** _pcData8);

/**
 * Get integer 16 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData16 return pointer on 16 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfInteger16(int* _piAddress, int* _piRows, int* _piCols, short** _psData16);

/**
 * Get integer 32 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData32 return pointer on 32 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
int getMatrixOfInteger32(int* _piAddress, int* _piRows, int* _piCols, int** _piData32);

/**
 * Get integer 64 bit variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _piData64 return pointer on 64 bits integer
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int getMatrixOfInteger64(int* _piAddress, int* _piRows, int* _piCols, long long** _pllData64);
#endif

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData8 array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfInteger8(int _iVar, int _iRows, int _iCols, char* _pcData8);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData16 array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfInteger16(int _iVar, int _iRows, int _iCols, short* _psData16);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData32 array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int createMatrixOfInteger32(int _iVar, int _iRows, int _iCols, int* _piData32);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData64 array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */

#ifdef __SCILAB_INT64__
int createMatrixOfInteger64(int _iVar, int _iRows, int _iCols, long long* _pllData64);
#endif

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData8 return array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int allocMatrixOfInteger8(int _iVar, int _iRows, int _iCols, char** _pcData8);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData16 return array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int allocMatrixOfInteger16(int _iVar, int _iRows, int _iCols, short** _psData16);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData32 return array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int allocMatrixOfInteger32(int _iVar, int _iRows, int _iCols, int** _piData32);

/**
 * Get integer variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _piData64 return array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int allocMatrixOfInteger64(int _iVar, int _iRows, int _iCols, long long** _pllData64);
#endif

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData8 array of integer 8 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMatrixOfInteger8(char* _pstName, int _iRows, int _iCols, char* _pcData8);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData16 array of integer 16 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMatrixOfInteger16(char* _pstName, int _iRows, int _iCols, short* _psData16);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData32 array of integer 32 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
int createNamedMatrixOfInteger32(char* _pstName, int _iRows, int _iCols, int* _piData32);

/**
 * Get integer variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _piData64 array of integer 64 bits
 * @param[out] _piAddress return pointer on new variable
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int createNamedMatrixOfInteger64(char* _pstName, int _iRows, int _iCols, long long* _pllData64);
#endif

/**
 * Get integer precision ( 8, 16, 32, 64 )
 * @param[in] _pstName variable name
 * @param[out] _piPrecison return integer precision ( 8, 1-, 32, 64 )
 */
int getNamedMatrixOfIntegerPrecision(char* _pstName, int* _piPrecision);

/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData8 return array of integer 8 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMatrixOfInteger8(char* _pstName, int* _piRows, int* _piCols, char* _pcData8);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData16 return array of integer 16 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMatrixOfInteger16(char* _pstName, int* _piRows, int* _piCols, short* _psData16);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData32 return array of integer 32 bits
 * @return if the operation successed (0) or not ( !0 )
 */
int readNamedMatrixOfInteger32(char* _pstName, int* _piRows, int* _piCols, int* _piData32);
/**
 * Get integer named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _piData64 return array of integer 64 bits
 * @return if the operation successed (0) or not ( !0 )
 */
#ifdef __SCILAB_INT64__
int readNamedMatrixOfInteger64(char* _pstName, int* _piRows, int* _piCols, long long* _pllData64);
#endif
#endif /* __INT_API__ */
