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
#include "dynlib_api_scilab.h"

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
API_SCILAB_IMPEXP int getMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getComplexMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);

/**
 * Get double variable data
 * @param[in] _piAddress variable address
 * @param[out] _piRows return number of row 
 * @param[out] _piCols return number of column
 * @param[out] _pdblZ return pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int getComplexZMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int allocMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[out] _pdblReal return pointer on real data
 * @param[out] _pdblImg return pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int allocComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);

/**
 * Create double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * Get double variable data
 * @param[in] _iVar variable number
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @param[in] _pdblZ pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createComplexZMatrixOfDouble(int _iVar, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedMatrixOfDouble(char* _pstName, int _iRows, int _iCols, double* _pdblReal);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblReal pointer on real data
 * @param[in] _pdblImg pointer on img data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedComplexMatrixOfDouble(char* _pstName, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);

/**
 * Get double variable data
 * @param[in] _pstName variable name
 * @param[in] _iRows Number of row
 * @param[in] _iCols Number of column
 * @param[in] _pdblZ pointer on Z format data ( Real1, Img1, Real2, Img2, ... )
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int createNamedComplexZMatrixOfDouble(char* _pstName, int _iRows, int _iCols, doublecomplex* _pdblData);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedMatrixOfDouble(char* _pstName, int* _piRows, int* _piCols, double* _pdblReal);

/**
 * Get double named variable data
 * @param[in] _pstName variable name
 * @param[out] _piRows return number of row
 * @param[out] _piCols return number of column
 * @param[out] _pdblReal return real data
 * @param[out] _pdblImg return imgagianry data
 * @return if the operation successed (0) or not ( !0 )
 */
API_SCILAB_IMPEXP int readNamedComplexMatrixOfDouble(char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);


#ifdef __cplusplus
}
#endif
#endif /* __DOUBLE_API__ */
