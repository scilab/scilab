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

#ifndef __DOUBLE_API__
#define __DOUBLE_API__

#ifdef __cplusplus
extern "C" {
#endif

#include "doublecomplex.h"

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
StrErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblZ return pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

/**
 * Create double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * Get complex double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[ ] _pdblData pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblData pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr createNamedComplexZMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr readNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
StrErr readNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_API__ */
